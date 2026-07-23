#ifndef FRAME_H
#define FRAME_H

#include <chrono>
#include <cstdint>
#include <memory>

#include "CameraTypes.h"
#include "IFrameBuffer.h"


namespace OIS::Core
{

struct FrameMetadata
{
    std::uint64_t frameNumber{0};

    std::chrono::steady_clock::time_point timestamp;

    std::uint32_t width{0};

    std::uint32_t height{0};

    std::uint32_t stride{0};

    PixelFormat pixelFormat{PixelFormat::Unknown};
};

struct Frame
{
    CameraId cameraId{0};

    FrameMetadata metadata;

    std::shared_ptr<const IFrameBuffer> buffer;

    bool isValid() const noexcept
    {
        return cameraId != 0 && buffer != nullptr;
    }
};

}
#endif // FRAME_H
