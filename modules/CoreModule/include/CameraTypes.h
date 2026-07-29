#ifndef CAMERATYPES_H
#define CAMERATYPES_H

#include <cstdint>
#include <string>


namespace OIS::Core
{

using CameraId = std::uint64_t;

enum class CameraTechnology
{
    GigE,
    USB3Vision,
    CameraLink,
    CoaXPress
};

enum class CameraState
{
    Unknown,
    Disconnected,
    Connected,
    Grabbing,
    Error
};

enum class PixelFormat
{
    Unknown,

    Mono8,
    Mono10,
    Mono12,
    Mono16,

    BayerRG8,
    BayerRG12,

    RGB8,
    BGR8,

    YUV422
};

enum class TriggerMode
{
    Continuous,
    Software,
    Hardware
};

struct CameraConfig
{
    CameraId id{0};

    CameraTechnology technology{ CameraTechnology::GigE };

    std::string serialNumber;

    std::string ipAddress;

    std::uint32_t width{ 0 };

    std::uint32_t height{ 0 };

    PixelFormat pixelFormat{ PixelFormat::Unknown };

    double fps{ 0.0 };

    TriggerMode triggerMode{ TriggerMode::Continuous };

    double exposure{ 0.0 };

    double gain{ 0.0 };
};

}

#endif // CAMERATYPES_H
