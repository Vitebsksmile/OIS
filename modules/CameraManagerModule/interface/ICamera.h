#ifndef ICAMERA_H
#define ICAMERA_H

#include "CameraTypes.h"


class IFrameObserver;

class ICamera
{
public:
    virtual ~ICamera() = default;

    virtual bool open() = 0;
    virtual void close() = 0;

    virtual bool startGrab() = 0;
    virtual void stopGrab() = 0;

    virtual void setObserver(IFrameObserver* observer) = 0;

    virtual OIS::Core::CameraState state() = 0;
};

#endif // ICAMERA_H
