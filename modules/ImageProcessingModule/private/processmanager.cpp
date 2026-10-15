#include "processmanager.h"
#include <QDebug>
#include "ImageProcessingService.h"
#include "imagepreprocessing.h"  //???
//#include "yolodetectionworker.h"

ProcessManager::ProcessManager(ImageProcessingService *imageProcessingService, QObject *parent)
    : QObject(parent)
    , m_service(imageProcessingService)
    , m_workerThread(new QThread(this))
    , m_worker(new YoloDetectionWorker("D:/QtProjects/OIS/models/best.onnx"))
{
    if (!m_service) {
        qWarning() << "ProcessManager: processManager created without reference to facade";
    } else {
        qDebug() << "ProcessManager: ProcessManager object created; parent:"
                 << parent;
    }

    m_worker->moveToThread(m_workerThread);
    connect(m_workerThread, &QThread::started,
            m_worker, &YoloDetectionWorker::initialize);
    connect(m_workerThread, &QThread::finished,
            m_worker, &QObject::deleteLater);
    connect(m_worker, &YoloDetectionWorker::detectionsReady,
            this, &ProcessManager::onDetectionsReady);

    connect(m_worker, &YoloDetectionWorker::detectionFinished,
            this, &ProcessManager::onDetectionFinished);
    connect(m_worker, &YoloDetectionWorker::errorOccurred,
            this, &ProcessManager::onWorkerError);

    m_workerThread->start();

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

    connect(this, &ProcessManager::cropPCBReady,
            m_service, &ImageProcessingService::onCropPCBReady);
}

ProcessManager::~ProcessManager()
{
    if (m_workerThread && m_workerThread->isRunning()) {
        m_workerThread->quit();
        m_workerThread->wait();
    }
}

void ProcessManager::toggle()
{
    qDebug() << "ProcessManager::toggle()";
    m_triggered.store(true);
}

//  --- PUBLIC SLOTS ---
//  Слушает фасад для старта предобработки
void ProcessManager::onImagePreProcessingRequested(const QString &filePath)  //???
{
    //  Создание объекта ImagePreProcessing
    createPreProcessingObject();

    // Установка изображения в обработчик
    if (imagePreProcessing()->loadImage(filePath)) {
        qDebug() << "ProcessManager: The file path is valid and contains an image. "
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
    if (cvFrame.empty()) { return; }

    // Фиксируем изображение для текущей итерации.
    const cv::Mat sourceFrame = cvFrame.clone();

    m_processing = std::make_unique<FrameProcessing>(sourceFrame.clone());
    m_processing->toGray().gaussianBlur().closesGapsInLines();

    emit processedFrameReady(this->matToQImage(m_processing->cvFrame()));

    m_finder = std::make_unique<ObjectsFinder>(m_processing->cvFrame());
    m_finder->findContours().detectRaggedRectangularObjects().makeBoundRect();

    emit frameWithBoxesReady(this->matToQImage(sourceFrame),
                             m_finder->rectanglePoints());

    const auto contoursCopy = m_finder->contours();
    if (contoursCopy.size() == 1 && m_triggered) {
        if (!m_worker || !m_workerThread || !m_workerThread->isRunning()) {
            qDebug() << "ProcessManager: exit from method ProcessManager::onProcessFrame()";
            return;
        }

        // Не накапливаем очередь кадров, если YOLO занят.
        bool expected = false;
        if (!m_yoloBusy.compare_exchange_strong(expected, true)) {
            qDebug() << "ProcessManager: exit from method ProcessManager::onProcessFrame()"
                        "m_yoloBusy == false";
            return;
        }

        cv::Mat cvCropPCB = m_processing->cropAndCorrectPCB_5(sourceFrame, contoursCopy.front());
        if (cvCropPCB.empty()) {
            qWarning() << "ProcessManager: failed to crop PCB";
            m_triggered = false;
            return;
        }

        // qDebug() << "Crop size:" << cvCropPCB.cols << "x" << cvCropPCB.rows
        //          << "aspect ratio:"
        //          << static_cast<double>(cvCropPCB.cols) / cvCropPCB.rows;

        YoloDetectionWorker *worker = m_worker;
        const bool invoked = QMetaObject::invokeMethod(worker, [this, cvFrame = std::move(cvCropPCB)] () {
            m_worker->processFrame(cvFrame);
        }, Qt::QueuedConnection);

        if (!invoked) {
            m_yoloBusy.store(false);
            qWarning() << "ProcessManager: failed to submit YOLO frame";
        }

        m_triggered = false;
    }
}

void ProcessManager::onDetectionFinished()
{
    m_yoloBusy.store(false);
}

void ProcessManager::onWorkerError(const QString &message)
{
    qWarning() << "ProcessManager: YOLO worker ERROR:" << message;
}

void ProcessManager::onDetectionsReady(const cv::Mat &mat,
                                       const QVector<Core::Detection> &detections)
{
    emit detectionsReady(this->matToQImage(mat), detections);
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
                      rgb.step, //  шаг строки (stride) - полное количество байт в одной строке матрицы, включая техническое выравнивание памяти
                      QImage::Format_RGBA8888).copy();    //  формат цвета
    }
    return QImage();
}

QImage ProcessManager::matToQImage_2(const cv::Mat &mat)
{
    switch (mat.type()) {

    // 8-bit, 1 channel (Grayscale)
    case CV_8UC1: {
        QImage image(mat.data,
                     mat.cols,
                     mat.rows,
                     static_cast<int>(mat.step), // ИСПРАВЛЕНИЕ ТУТ
                     QImage::Format_Grayscale8);
        return image.copy();
    }

    // 8-bit, 3 channel (RGB / BGR)
    case CV_8UC3: {
        QImage image(mat.data,
                     mat.cols,
                     mat.rows,
                     static_cast<int>(mat.step), // ИСПРАВЛЕНИЕ ТУТ: явно задаем шаг строки
                     QImage::Format_RGBA8888);
        return image.rgbSwapped(); // OpenCV хранит BGR, а Qt нужен RGB
    }

    // 8-bit, 4 channel (RGBA)
    case CV_8UC4: {
        QImage image(mat.data,
                     mat.cols,
                     mat.rows,
                     static_cast<int>(mat.step), // ОБЯЗАТЕЛЬНО передаем inMat.step!
                     QImage::Format_ARGB32);
        return image.copy(); // .copy() делает глубокое копирование данных
    }

    default:
        break;
    }
    return QImage();
}

QImage ProcessManager::cvMatToQImagePerfect(const cv::Mat& inMat) {
    if (inMat.empty()) return QImage();

    // 1. Переводим исходную матрицу в 4-канальный RGBA (или BGRA) формат
    cv::Mat rgbaMat;
    if (inMat.channels() == 3) {
        // Если на входе BGR (3 канала), переводим в RGBA (4 канала)
        cv::cvtColor(inMat, rgbaMat, cv::COLOR_BGR2RGBA);
    } else if (inMat.channels() == 1) {
        // Если на входе черно-белое, переводим в RGBA
        cv::cvtColor(inMat, rgbaMat, cv::COLOR_GRAY2RGBA);
    } else if (inMat.channels() == 4) {
        // Если уже 4 канала, проверяем порядок (OpenCV обычно хранит BGRA)
        cv::cvtColor(inMat, rgbaMat, cv::COLOR_BGRA2RGBA);
    } else {
        return QImage();
    }

    // 2. ЖЕСТКАЯ СТРУКТУРИЗАЦИЯ ПАМЯТИ (Защита от диагонального сдвига)
    // Вычисляем идеальный шаг строки для RGBA: Ширина * 4 байта (на пиксель)
    int idealStep = rgbaMat.cols * 4;

    // Создаем гарантированно "плотную" матрицу без скрытых байтов отступа OpenCV
    cv::Mat forcedMat(rgbaMat.rows, rgbaMat.cols, CV_8UC4);

    // Построчно копируем пиксели из rgbaMat в forcedMat
    for (int i = 0; i < rgbaMat.rows; ++i) {
        std::memcpy(forcedMat.ptr(i), rgbaMat.ptr(i), idealStep);
    }

    // 3. Создаем QImage на основе нашей выровненной forcedMat.
    // Для RGBA в Qt используется формат Format_ARGB32 или Format_RGBA8888.
    QImage image(forcedMat.data,
                 forcedMat.cols, forcedMat.rows,
                 static_cast<int>(forcedMat.step),
                 QImage::Format_ARGB32);

    // 4. Делаем глубокое копирование .copy().
    // Это изолирует память QImage от временного объекта forcedMat,
    // предотвращая появление мусора при уничтожении переменной forcedMat.
    return image.copy();
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



// void ProcessManager::startDetection(const cv::Mat &cvFrame)
// {
//     QMetaObject::invokeMethod(m_worker, [this, cvFrame] () {
//         m_worker->startCapture(cvFrame);
//     }, Qt::QueuedConnection);
// }

// OIS::Core::Frame ProcessManager::matToFrame(const cv::Mat &mat) const
// {
//     if (mat.type() == CV_8UC3) {
//         cv::Mat rgb;
//         cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);

//         return Frame()
//     }
// }
