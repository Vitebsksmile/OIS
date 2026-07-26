#ifndef MOCKFRAMEBUFFER_H
#define MOCKFRAMEBUFFER_H

#include <vector>

#include "IFrameBuffer.h"


class MockFrameBuffer final
    : public OIS::Core::IFrameBuffer
{
public:

    MockFrameBuffer(
        std::uint32_t width,
        std::uint32_t height,
        OIS::Core::PixelFormat format
        );

    const std::byte*
    data() const noexcept override;

    std::size_t
    size() const noexcept override;

    std::uint32_t
    width() const noexcept override;

    std::uint32_t
    height() const noexcept override;

    std::uint32_t
    stride() const noexcept override;

    OIS::Core::PixelFormat
    pixelFormat() const noexcept override;

private:

    std::vector<std::byte> m_data;

    std::uint32_t m_width;

    std::uint32_t m_height;

    OIS::Core::PixelFormat m_pixelFormat;
};

#endif // MOCKFRAMEBUFFER_H
