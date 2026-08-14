#include "processmanager.h"
#include <QDebug>
#include "IImageProcessingService.h"
#include "imagepreprocessing.h"


ProcessManager::ProcessManager(
    IImageProcessingService *imageProcessingService
    , QObject *parent)
    : QObject(parent)
    , m_imageProcessingService(imageProcessingService)
{
    if (!m_imageProcessingService)
    {
        qWarning()
            << "ProcessManager: processManager created without reference to facade";
    }

    qDebug()
        << "ProcessManager: ProcessManager object created; parent: "
        << parent;

    //  Слушает фасад для старта предобработки
    connect(m_imageProcessingService, &IImageProcessingService::imagePreProcessingRequested,
            this, &ProcessManager::onImagePreProcessingRequested);

    //  To Facade for QML about Start
    connect(this, &ProcessManager::preProcessingStartNotification,
            m_imageProcessingService, &IImageProcessingService::onPreProcessingStartNotification);

    //  To Facade fot QML about Finished
    connect(this, &ProcessManager::preProcessingFinished,
            m_imageProcessingService, &IImageProcessingService::onPreProcessingFinished);

    //  The facade listens for the appearance of a frame
    connect(m_imageProcessingService, &IImageProcessingService::processFrame
            , this, &ProcessManager::onProcessFrame);

    //  this -> ImageProcessingService
    connect(this, &ProcessManager::frameWithBoxesReady
            , m_imageProcessingService, &IImageProcessingService::onFrameWithBoxesReady);
}


//  --- PUBLIC SLOTS ---
//  Слушает фасад для старта предобработки
void ProcessManager::onImagePreProcessingRequested(const QString &filePath)
{
    //  Создание объекта ImagePreProcessing
    createPreProcessingObject();

    // Установка изображения в обработчик
    if (imagePreProcessing()->loadImage(filePath))
    {
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
    m_processing->toGray().gaussianBlur(5).toBinary();

    m_finder = std::make_unique<ObjectFinder>(m_processing->cvFrame());
    m_finder->findObjects();
    emit frameWithBoxesReady(this->matToQImage(cvFrame)
                             , m_finder->rectanglePoints());
}
//  --- END PUBLIC SLOTS ---

//  Создает объект ImagePreprocessing
void ProcessManager::createPreProcessingObject()
{
    //  Старый объект удалиться сам при вызове reset()
    m_imagePreProcessing = std::make_unique<ImagePreProcessing>();

    qDebug()
        << "ProcessManager: Created an ImagePreProcessing object at: "
        << m_imagePreProcessing.get();
}

void ProcessManager::deletePreProcessingObject()
{
    // Просто сбрасываем указатель.
    // Деструктор ~ImagePreProcessing() вызовется немедленно.
    m_imagePreProcessing.reset();

    qDebug()
        << "ProcessManager: The ImagePreProcessing object was manually destroyed. "
           "The m_imagePreProcessing pointer: "
        << m_imagePreProcessing.get();
}

void ProcessManager::usePreProcessing(ImagePreProcessing *preProcessing)
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
    if (preProcessing->save())
    {
        emit preProcessingFinished(preProcessing->finalFilePath());
    }
}

QImage ProcessManager::matToQImage(const cv::Mat &mat)
{
    if (mat.type() == CV_8UC3) {
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
