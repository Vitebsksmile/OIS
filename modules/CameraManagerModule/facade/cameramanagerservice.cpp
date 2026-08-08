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

    connect(&m_workerThread, &QThread::finished,
            m_worker, &QObject::deleteLater);

    connect(m_worker, &VideoCaptureWorker::frameReady,
            this, &CameraManagerService::onFrameReady);

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

void CameraManagerService::onFrameReady(const QImage frame)
{
    emit frameReady(frame);
}

void CameraManagerService::startStream()
{
    QMetaObject::invokeMethod(m_worker, [this] () {
        m_worker->startCaptureUrl();
    }, Qt::QueuedConnection );
}
