#ifndef IFRAMEBUFFER_H
#define IFRAMEBUFFER_H

#include <cstddef>
#include <cstdint>

#include "CameraTypes.h"


namespace OIS::Core
{

class IFrameBuffer
{
public:
    virtual ~IFrameBuffer() = default;

    virtual const std::byte* data() const noexcept = 0;

    virtual std::size_t size() const noexcept = 0;

    virtual std::uint32_t width() const noexcept = 0;

    virtual std::uint32_t height() const noexcept = 0;

    virtual std::uint32_t stride() const noexcept = 0;

    virtual PixelFormat pixelFormat() const noexcept = 0;
};

}

#endif // IFRAMEBUFFER_H
