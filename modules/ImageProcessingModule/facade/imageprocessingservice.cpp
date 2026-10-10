/*
 * Этот файл реализует асинхронную логику:
 * здесь Qt (управление потоками) встречается с OpenCV (математика изображений).
 * Главная фишка кода в том, что интерфейс не «зависает» во время обработки.
*/
/*
 * Почему это написано грамотно:
 * Thread Safety: Сигнал finished у QFutureWatcher всегда приходит в тот поток, где был создан объект (обычно в главный UI-поток). Это позволяет безопасно обновлять интерфейс.
 * Лямбда-захват: В QtConcurrent::run передается filePath по значению, что безопасно для многопоточности.
 * Инкапсуляция: Сложная математика (performProcessing) отделена от логики загрузки и потоков.
*/

#include "imageprocessingservice.h"
#include <QDebug>
#include <QThread>
#include "processmanager.h"

ImageProcessingService::ImageProcessingService(QObject *parent)
    : IImageProcessingService(parent),
    m_procManager(new ProcessManager (this, this))
{
    connect(m_procManager, &ProcessManager::detectionsReady,
        this, &ImageProcessingService::detectionsReady);
}

bool ImageProcessingService::setDbService(IDatabaseService *dbService)
{
    m_dbService = dbService;
    if (m_dbService) { return true; }
    return false;
}

bool ImageProcessingService::setCamService(ICameraManagerService *camService)
{
    m_camService = camService;
    bool ok = false;
    ok = connect(m_camService, &ICameraManagerService::rawCVFrameReady,
                 this, &ImageProcessingService::onRawCVFrameReady);
    if (!ok) {
        qCritical()
        << "WARNING! ImageProcessingService: Failed to subscribe to CameraManagerService signals";
        return false;
    }
    return true;
}

void ImageProcessingService::readFrame(bool isRead)
{
    m_procManager->setFlag_detections(isRead);
}

//  Слот для получения пути из VisualizationModule
//  Запуск обработки
void ImageProcessingService::onImagePreProcessingRequested(const QString &filePath)  //???
{
    //  Базовая проверка: если путь пустой, то сразу выходим с ошибкой
    if (filePath.isEmpty())
    {
        qDebug()
            << "ImageProcessingService: The file path is empty! Path to image: "
            << filePath;

        emit prePreProcessingError(filePath, "Empty file path");

        return;
    }

    //  Запоминаем путь, чтобы потом передать его в сигнале завершения
    m_currentFilePath = filePath;

    qDebug()
        << "ImageProcessingService: The file path has been obtained! Path to image: "
        << filePath
        << ". I pass it to ProcessManager.";

    emit imagePreProcessingRequested(filePath);
}

//  Слушает ProcessManager для дальнейшей отправки в VisualizationModule
//  для уведомления User о начале предобработки (for QML about Start)
void ImageProcessingService::onPreProcessingStartNotification(bool success) {}  //???

//  Слушает ProcessManager для дальнейшей отправки в VisualizationModule
//  для уведомления о завершении предобработки (for QML about Finished)
void ImageProcessingService::onPreProcessingFinished(const QString &resultFilePath)  //???
{
    imagePreProcessingFinished("file:///" + resultFilePath, true);
    qDebug() << "ImageProcessingService: "
                "Preprocessing completion signal sent; "
                "resultFilePath: "
             << resultFilePath;
}

//  CameraManagerService -> this
void ImageProcessingService::onRawCVFrameReady(const cv::Mat &cvFrame)
{
    //  this -> ProcessManager
    emit processFrame(cvFrame, m_detectionMethod);
}

//  ProcessManager -> this -------delete
void ImageProcessingService::onProcessedFrameReady(const QImage &frame)
{
    emit processedFrameReady(frame);
}

//  ProcessManager -> this -------delete
void ImageProcessingService::onFrameWithBoxesReady(const QImage &frame,
                                                   const std::vector<std::vector<int>> &rectanglePoints)
{
    //  this -> VisualizationService
    emit frameWithBoxesReady(frame,
                             rectanglePoints);

    m_dbService -> logNewDefect(777, 1, "R105", 20, 45);
}

void ImageProcessingService::onDetectionsReady(const QImage &frame, const QVector<Core::Detection> &detections)
{
    emit detectionsReady(frame, detections);
}

//  Factory method
QSharedPointer<IImageProcessingService> createImageProcessingService(QObject* parent)
{
    return QSharedPointer<IImageProcessingService>(new ImageProcessingService(parent));
}
