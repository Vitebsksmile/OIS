#include "mockframebuffer.h"


MockFrameBuffer::MockFrameBuffer(
    std::uint32_t width,
    std::uint32_t height,
    OIS::Core::PixelFormat format
    )
    : m_data(
          static_cast<std::size_t>(
              width
              ) *
          static_cast<std::size_t>(
              height
              )
          )
    , m_width(width)
    , m_height(height)
    , m_pixelFormat(format)
{
}


const std::byte*
MockFrameBuffer::data() const noexcept
{
    return m_data.data();
}


std::size_t
MockFrameBuffer::size() const noexcept
{
    return m_data.size();
}


std::uint32_t
MockFrameBuffer::width() const noexcept
{
    return m_width;
}


std::uint32_t
MockFrameBuffer::height() const noexcept
{
    return m_height;
}


std::uint32_t
MockFrameBuffer::stride() const noexcept
{
    return m_width;
}


OIS::Core::PixelFormat
MockFrameBuffer::pixelFormat() const noexcept
{
    return m_pixelFormat;
}
