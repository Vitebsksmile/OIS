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
}

CameraManagerService::~CameraManagerService()
{
    m_workerThread.quit();
    m_workerThread.wait();
}

bool CameraManagerService::creatCamera(const QString &url)
{
    qDebug()
        << "CameraManagerService: creatCamera with URL address:"
        << url;

    m_worker->moveToThread(&m_workerThread);

    connect(&m_workerThread, &QThread::finished
            , m_worker, &QObject::deleteLater);

    connect(m_worker, &VideoCaptureWorker::rawCVFrameReady
            , this, &CameraManagerService::onRawCVFrameReady);

    connect(m_worker, &VideoCaptureWorker::rawImageFrameReady
            , this, &CameraManagerService::onRawImageFrameReady);

    connect (m_worker, &VideoCaptureWorker::customFrameReady
            , this, &CameraManagerService::onCustomFrameReady);

    m_workerThread.start();

    this->startStream(url);

    return true;
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
void CameraManagerService::onRawCVFrameReady(const cv::Mat &cvFrame)
{
    //  this -> ImageProcessingService
    //emit rawCVFrameReady(cvFrame.clone());
    emit rawCVFrameReady(cvFrame);
}

//  VideoCaptureWorker -> this
void CameraManagerService::onRawImageFrameReady(const QImage &frame)
{
    //  this -> VisualizationService
    emit rawImageFrameReady(frame);
}

void CameraManagerService::onCustomFrameReady(const CVFrameBuffer &frame)
{
    //----------------------
}

void CameraManagerService::startStream()
{
    QMetaObject::invokeMethod(m_worker, [this] () {
        m_worker->startCaptureUrl();
    }, Qt::QueuedConnection );
}

void CameraManagerService::startStream(const QString &url)
{
    QMetaObject::invokeMethod(m_worker, [this, url] () {
        m_worker->startCaptureUrl(url);
    }, Qt::QueuedConnection );
}

//  Factory method
QSharedPointer<ICameraManagerService> createCameraManagerService(QObject* parent)
{
    return QSharedPointer<ICameraManagerService>(new CameraManagerService(parent));
}
