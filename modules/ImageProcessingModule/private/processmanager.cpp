#include "processmanager.h"
#include <QDebug>
#include "ImageProcessingService.h"
#include "imagepreprocessing.h"  //???
//#include "yolodetectionworker.h"

ProcessManager::ProcessManager(ImageProcessingService *imageProcessingService, QObject *parent)
    : QObject(parent)
    , m_service(imageProcessingService)
    , m_worker(new YoloDetectionWorker("D:/QtProjects/OIS/models/best.onnx"))
{
    if (!m_service) {
        qWarning()
            << "ProcessManager: processManager created without reference to facade";
    } else {
        qDebug()
            << "ProcessManager: ProcessManager object created; parent:"
            << parent;
    }

    m_worker->moveToThread(&m_workerThread);
    connect(&m_workerThread, &QThread::finished,
            m_worker, &QObject::deleteLater);
    connect(m_worker, &YoloDetectionWorker::detectionsReady,
            this, &ProcessManager::onDetectionsReady);

    //  Слушает фасад для старта предобработки
    connect(m_service, &ImageProcessingService::imagePreProcessingRequested,
            this, &ProcessManager::onImagePreProcessingRequested);  //???

    //  To Facade for QML about Start
    connect(this, &ProcessManager::preProcessingStartNotification,
            m_service, &ImageProcessingService::onPreProcessingStartNotification);  //???

    //  To Facade fot QML about Finished
    connect(this, &ProcessManager::preProcessingFinished,
            m_service, &ImageProcessingService::onPreProcessingFinished);  //???

    //  The facade listens for the appearance of a frame
    connect(m_service, &ImageProcessingService::processFrame,
            this, &ProcessManager::onProcessFrame);

    //  this -> ImageProcessingService
    connect(this, &ProcessManager::processedFrameReady,
            m_service, &ImageProcessingService::onProcessedFrameReady);

    //  this -> ImageProcessingService
    connect(this, &ProcessManager::frameWithBoxesReady,
            m_service, &ImageProcessingService::onFrameWithBoxesReady);
}

ProcessManager::~ProcessManager()
{
    m_workerThread.quit();
    m_workerThread.wait();
}

void ProcessManager::setFlag_detections(bool flag)
{
    m_flag_detections = flag;
}

//  --- PUBLIC SLOTS ---
//  Слушает фасад для старта предобработки
void ProcessManager::onImagePreProcessingRequested(const QString &filePath)  //???
{
    //  Создание объекта ImagePreProcessing
    createPreProcessingObject();

    // Установка изображения в обработчик
    if (imagePreProcessing()->loadImage(filePath)) {
        qDebug()
        << "ProcessManager: The file path is valid and contains an image. "
           "The image has been installed in the ImagePreProcessing handler! "
           "Path to image: "
        << filePath;
    }

    //  Используем объект
    usePreProcessing(imagePreProcessing());

    //  Удаляем объект
    deletePreProcessingObject();
}

//  ImageProcessingService -> this
void ProcessManager::onProcessFrame(const cv::Mat &cvFrame)
{
    cv::Mat localFrame = cvFrame;

    m_processing = std::make_unique<FrameProcessing>(localFrame);
    //m_processing->toGray().gaussianBlur(1).toBinary();
    m_processing->toGray().gaussianBlur().closesGapsInLines();//.marksBoundaries()

    emit processedFrameReady(this->matToQImage(m_processing->cvFrame()));

    m_finder = std::make_unique<ObjectFinder>(m_processing->cvFrame());
    m_finder->findObjects();

    emit frameWithBoxesReady(this->matToQImage(cvFrame),
                             m_finder->rectanglePoints());

    if (m_flag_detections) {
        m_workerThread.start();
        this->startDetection(cvFrame);
    }
}

void ProcessManager::onDetectionsReady(const QImage &frame, const QVector<Core::Detection> &detections)
{
    emit detectionsReady(frame, detections);
}
//  --- END PUBLIC SLOTS ---

//  Создает объект ImagePreprocessing
void ProcessManager::createPreProcessingObject()  //???
{
    //  Старый объект удалиться сам при вызове reset()
    m_imagePreProcessing = std::make_unique<ImagePreProcessing>();

    qDebug()
        << "ProcessManager: Created an ImagePreProcessing object at: "
        << m_imagePreProcessing.get();
}

void ProcessManager::deletePreProcessingObject()  //???
{
    // Просто сбрасываем указатель.
    // Деструктор ~ImagePreProcessing() вызовется немедленно.
    m_imagePreProcessing.reset();

    qDebug()
        << "ProcessManager: The ImagePreProcessing object was manually destroyed. "
           "The m_imagePreProcessing pointer: "
        << m_imagePreProcessing.get();
}

void ProcessManager::usePreProcessing(ImagePreProcessing *preProcessing)  //???
{
    //  To Facade for QML
    emit preProcessingStartNotification(true);

    //  Используем метод обработки
    preProcessing->toGray();
    preProcessing->gaussianBlur(10);

    qDebug()
        << "ProcessManager: Pre-processing complete. Channels: "
        << (preProcessing->getResult()).channels();

    //  Сохранение результатов на диск
    if (preProcessing->save()) {
        emit preProcessingFinished(preProcessing->finalFilePath());
    }
}

QImage ProcessManager::matToQImage(const cv::Mat &mat)
{
    if (mat.type() == CV_8UC1) {

        return QImage(mat.data,
                      mat.cols,
                      mat.rows,
                      mat.step,
                      QImage::Format_Grayscale8).copy();

    } else if (mat.type() == CV_8UC3) {
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGBA);

        //  Создаем QImage и принудительно копируем данные в кучу Qt,
        //  чтобы безопасно передать изображение через потоки.
        return QImage(rgb.data, //  сырой указатель на первый байт в памяти, где лежит матрица пикселей изображения
                      rgb.cols, //  количество столбцов матрицы
                      rgb.rows, //  количество строк матрицы
                      rgb.step, // шаг строки (stride) - полное количество байт в одной строке матрицы, ключая техническое выравнивание памяти
                      QImage::Format_RGBA8888).copy();    //  формат цвета
    }
    return QImage();
}

QImage ProcessManager::matToGrayQImage(const cv::Mat &mat)
{
    // Проверяем, что матрица не пустая
    if (mat.empty()) {
        return QImage();
    }

    // Если это одноканальное изображение (GrayScale или Binary после .toBinary())
    if (mat.type() == CV_8UC1) {
        return QImage(
                   reinterpret_cast<const uchar*>(mat.data),
                   mat.cols,
                   mat.rows,
                   static_cast<int>(mat.step),
                   QImage::Format_Grayscale8
                   ).copy(); // Копируем данные в кучу Qt для безопасной передачи
    }
    // На случай, если сюда всё же придет цветное изображение (защитный код)
    if (mat.type() == CV_8UC3) {
        cv::Mat gray;
        cv::cvtColor(mat, gray, cv::COLOR_BGR2GRAY);
        return QImage(
                   reinterpret_cast<const uchar*>(gray.data),
                   gray.cols,
                   gray.rows,
                   static_cast<int>(gray.step),
                   QImage::Format_Grayscale8
                   ).copy();
    }

    return QImage();
}

void ProcessManager::startDetection(const cv::Mat &cvFrame)
{
    QMetaObject::invokeMethod(m_worker, [this, cvFrame] () {
        m_worker->startCapture(cvFrame);
    }, Qt::QueuedConnection);
}

void ProcessManager::stopDetection()
{
    QMetaObject::invokeMethod(m_worker, [this] () {
        m_worker->stopCapture();
    }, Qt::QueuedConnection );
}

// OIS::Core::Frame ProcessManager::matToFrame(const cv::Mat &mat) const
// {
//     if (mat.type() == CV_8UC3) {
//         cv::Mat rgb;
//         cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);

//         return Frame()
//     }
// }
