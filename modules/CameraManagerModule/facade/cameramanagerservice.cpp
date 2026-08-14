#include "cameramanagerservice.h"
#include <QDebug>
//#include "ICameraDriver.h"
//#include "baslerdriver.h"
#include "videocaptureworker.h"

CameraManagerService::CameraManagerService(QObject* parent)
    : ICameraManagerService(parent)
    , m_worker(new VideoCaptureWorker())
{
    qDebug() << "CameraManagerService: creat new object";
    //m_cameraDriver.reset(new BaslerDriver());

    m_worker->moveToThread(&m_workerThread);

    connect(&m_workerThread, &QThread::finished
            , m_worker, &QObject::deleteLater);

    connect(m_worker, &VideoCaptureWorker::imageFrameReady
            , this, &CameraManagerService::onImageFrameReady);

    connect(m_worker, &VideoCaptureWorker::cvFrameReady
            , this, &CameraManagerService::onCVFrameReady);

    m_workerThread.start();

    this->startStream();
}

CameraManagerService::~CameraManagerService()
{
    m_workerThread.quit();
    m_workerThread.wait();
}

// void CameraManagerService::checkAndConnectCamera()
// {
//     qDebug() << "CameraManagerService: Запуск сканирования камер";

//     CameraConfig config;

//     bool isConnected = m_cameraDriver->connect(config);

//     if (isConnected)
//     {
//         qDebug() << "CameraManagerService: Тестовое подключение прошло успешно.";
//         m_cameraDriver->disconnect();   //  Освобождаем камеру после теста
//     } else {
//         qWarning() << "CameraManagerService: Камера не обнаружена или произошда ошибка инициализации.";
//     }
// }

//  SLOT VideoCaptureWorker -> this
void CameraManagerService::onImageFrameReady(const QImage imageFrame)
{
    //  this -> VisualizationService
    emit imageFrameReady(imageFrame);
}

//  VideoCaptureWorker -> this
void CameraManagerService::onCVFrameReady(const cv::Mat &cvFrame)
{
    //  this -> ImageProcessingService
    emit cvFrameReady(cvFrame.clone());
}

void CameraManagerService::startStream()
{
    QMetaObject::invokeMethod(m_worker, [this] () {
        m_worker->startCaptureUrl();
    }, Qt::QueuedConnection );
}
