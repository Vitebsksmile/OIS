#ifndef CAMERAMANAGERSERVICE_H
#define CAMERAMANAGERSERVICE_H

#include <QObject>
//#include <QScopedPointer>
#include <memory>

#include "ICameraManagerService.h"


class ICameraDriver;

class CameraManagerService : public ICameraManagerService
{
    Q_OBJECT

public:

    explicit CameraManagerService(QObject* parent = nullptr);

    ~CameraManagerService() override;

    void checkAndConnectCamera();

public slots:


signals:

    void frameReady(OIS::Core::Frame &frame);

private:

    std::unique_ptr<ICameraDriver> m_cameraDriver;

};

#endif // CAMERAMANAGERSERVICE_H
