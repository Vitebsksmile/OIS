#ifndef MOCKCAMERAFACTORY_H
#define MOCKCAMERAFACTORY_H

#include <memory>

#include "ICameraFactory.h"


class MockCameraFactory final
    : public ICameraFactory
{
public:

    MockCameraFactory() = default;

    ~MockCameraFactory() override = default;

    std::unique_ptr<ICamera>
    createCamera(
        const OIS::Core::CameraConfig& config
        ) override;
};

#endif // MOCKCAMERAFACTORY_H
