#ifndef CAMERAMANAGERSERVICE_H
#define CAMERAMANAGERSERVICE_H

#include <QObject>

#include "ICameraManagerService.h"


class CameraManagerService : public ICameraManagerService
{
    Q_OBJECT

public:
    explicit CameraManagerService(QObject* parent = nullptr);

};

#endif // CAMERAMANAGERSERVICE_H
