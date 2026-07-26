#include "mockcamerafactory.h"
#include "mockcamera.h"



std::unique_ptr<ICamera>
MockCameraFactory::createCamera(
    const OIS::Core::CameraConfig& config
    )
{
    return std::make_unique<MockCamera>(
        config
        );
}