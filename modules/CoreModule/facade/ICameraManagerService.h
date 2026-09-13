#ifndef ICAMERAMANAGERSERVICE_H
#define ICAMERAMANAGERSERVICE_H

#include <QObject>
#include <QImage>

namespace cv { class Mat; }

class IFrameBuffer;

class ICameraManagerService : public QObject
{
    Q_OBJECT

public:
    explicit ICameraManagerService(QObject* parent = nullptr) : QObject(parent) {}

    virtual ~ICameraManagerService() = default;

    //virtual bool initialize() = 0;

    //virtual void shutdown() = 0;

    //virtual void startCamera(CameraId id) = 0;
    //virtual void stopCamera(CameraId id) = 0;

public slots:


signals:
    //void frameReady(const OIS::Core::Frame &frame);

    //  this -> VisualizationService
    void imageFrameReady(const QImage frame);

    //  this -> ImageProcessingService
    void cvFrameReady(const cv::Mat &frame);

    void frameReady(const IFrameBuffer &frame);
};

//  Factory method
QSharedPointer<ICameraManagerService> createCameraManagerService(QObject* parent);

#endif // ICAMERAMANAGERSERVICE_H
