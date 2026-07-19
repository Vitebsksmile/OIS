#ifndef ICAMERAMANAGERSERVICE_H
#define ICAMERAMANAGERSERVICE_H

#include <QObject>


class ICameraManagerService : public QObject
{
    Q_OBJECT

public:
    explicit ICameraManagerService(QObject* parent = nullptr) : QObject(parent) {}

    virtual ~ICameraManagerService() = default;

    virtual bool initialize() = 0;

    virtual void shutdown() = 0;

    //virtual void startCamera(CameraId id) = 0;
    //virtual void stopCamera(CameraId id) = 0;

public slots:

signals:

    //void frameReady(Frame frame);

};

#endif // ICAMERAMANAGERSERVICE_H
