#ifndef ICAMERA_H
#define ICAMERA_H

#include <QObject>


class ICamera : public QObject
{
    virtual ~ICamera() = default;

    virtual bool open() = 0;
    virtual void close() = 0;

    virtual bool startGrab() = 0;
    virtual void stopGrab() = 0;

    virtual void setObserver() = 0;

    //virtual CameraState state() = 0;
};

#endif // ICAMERA_H
