#ifndef CAMERAMANAGERSERVICE_H
#define CAMERAMANAGERSERVICE_H

#include <QObject>
//#include <QScopedPointer>
//#include <memory>
#include <QThread>
#include <QImage>

#include "ICameraManagerService.h"


//class ICameraDriver;
class VideoCaptureWorker;

class CameraManagerService : public ICameraManagerService
{
    Q_OBJECT

public:
    explicit CameraManagerService(QObject* parent = nullptr);

    ~CameraManagerService() override;

    void checkAndConnectCamera();

public slots:
    void startStream();

private slots:
    void onImageFrameReady(const QImage imageFrame);
    void onCVFrameReady(const cv::Mat &cvFrame);

signals:
    //  Сигналы объявленные в Интерфейсе в наследнике не объявляются, но используются!!!

private:
    //std::unique_ptr<ICameraDriver> m_cameraDriver;

    //QString m_streamUrl;
    //QImage m_currentFrame;
    QThread m_workerThread;
    VideoCaptureWorker *m_worker;
};

#endif // CAMERAMANAGERSERVICE_H
