#ifndef ICAMERADRIVER_H
#define ICAMERADRIVER_H

#include "cameraconfig.h"

class ICameraDriver
{
public:
    ICameraDriver() = default;
    virtual ~ICameraDriver() = default;

    virtual bool connect(const CameraConfig& config) = 0;
    virtual void disconnect() = 0;
    //virtual Frame grabFrame() = 0;
    //virtual void setParameter() = 0;
    //virtual Parameter parameter() = 0;

};

#endif // ICAMERADRIVER_H
