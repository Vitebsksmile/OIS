#ifndef ICAMERAMANAGERSERVICE_H
#define ICAMERAMANAGERSERVICE_H

#include <QObject>

#include "CameraTypes.h"
#include "Frame.h"


class ICameraManagerService : public QObject
{
    Q_OBJECT

public:

    explicit ICameraManagerService(QObject* parent = nullptr) : QObject(parent) {}

    virtual ~ICameraManagerService() override = default;

public slots:

    virtual bool initialize() = 0;

    virtual void shutdown() = 0;

    virtual OIS::Core::CameraId addCamera(const OIS::Core::CameraConfig& config) = 0;

    virtual bool removeCamera(OIS::Core::CameraId id) = 0;

    virtual bool startCamera(OIS::Core::CameraId id) = 0;

    virtual bool stopCamera(OIS::Core::CameraId id) = 0;

signals:

    void frameReady(OIS::Core::Frame frame);

    void cameraStateChanged(
        OIS::Core::CameraId id,
        OIS::Core::CameraState state
    );

    void errorOccured(
        OIS::Core::CameraId id,
        QString message
    );

};

#endif // ICAMERAMANAGERSERVICE_H
