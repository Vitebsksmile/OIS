#ifndef ICAMERAMANAGERSERVICE_H
#define ICAMERAMANAGERSERVICE_H

#include <QObject>

#include "Frame.h"

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

    virtual void onStartReady() = 0;

signals:

    void frameReady(const OIS::Core::Frame &frame);

};

#endif // ICAMERAMANAGERSERVICE_H
