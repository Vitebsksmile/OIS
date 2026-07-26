#ifndef CAMERAMANAGERSERVICE_H
#define CAMERAMANAGERSERVICE_H

#include <QObject>
#include <memory>
#include <unordered_map>

#include "CameraTypes.h"
#include "Frame.h"

#include "ICameraManagerService.h"
#include "IFrameObserver.h"


class ICamera;
class ICameraFactoryRegistry;

class CameraManagerService
    : public ICameraManagerService,
      public IFrameObserver
{
    Q_OBJECT

public:
    explicit CameraManagerService(std::unique_ptr<ICameraFactoryRegistry> factoryRegistry);

    ~CameraManagerService() override;

    bool initialize() override;

    void shutdown() override;

    OIS::Core::CameraId addCamera(const OIS::Core::CameraConfig& config) override;

    bool removeCamera(OIS::Core::CameraId id) override;

    bool startCamera(OIS::Core::CameraId id) override;

    void stopCamera(OIS::Core::CameraId id) override;

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

private:

    void onFrameReady(OIS::Core::Frame frame) override;

    std::unique_ptr<ICameraFactoryRegistry> m_factoryRegistry;

    std::unordered_map<
        OIS::Core::CameraId,
        std::unique_ptr<ICamera>
        > m_cameras;

};

#endif // CAMERAMANAGERSERVICE_H
