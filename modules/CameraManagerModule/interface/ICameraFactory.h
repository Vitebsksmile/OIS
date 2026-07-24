#ifndef ICAMERAFACTORY_H
#define ICAMERAFACTORY_H

#include <memory>

#include "CameraTypes.h"


class ICamera;

class ICameraFactory
{
public:
    virtual ~ICameraFactory() = default;

    virtual std::unique_ptr<ICamera> createCamera(
        const OIS::Core::CameraConfig& config
    ) = 0;
};

#endif // ICAMERAFACTORY_H
