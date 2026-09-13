#ifndef CVFRAMEBUFFER_H
#define CVFRAMEBUFFER_H

#include "IFrameBuffer.h"
#include <opencv2/opencv.hpp>

class CVFrameBuffer : public OIS::Core::IFrameBuffer
{
public:
    CVFrameBuffer() = default;
    explicit CVFrameBuffer(cv::Mat frame)
        : m_buffer(std::move(frame)) {}

    ~CVFrameBuffer() override = default;

    const std::byte* data() const noexcept override {
        if (m_buffer.empty()) {
            return nullptr;
        }
        return reinterpret_cast<const std::byte*>(m_buffer.data);
    }

    std::size_t size() const noexcept override {
        //  total() * elemSize() дает полный размер буфера в байтах с учетом всех каналов
        return m_buffer.total() * m_buffer.elemSize();
    }

    std::uint32_t width() const noexcept override {
        return static_cast<std::uint32_t>(m_buffer.cols);
    }

    std::uint32_t height() const noexcept override {
        return static_cast<std::uint32_t>(m_buffer.rows);
    }

    std::uint32_t stride() const noexcept override {
        // step[0] в OpenCV — это как раз количество байт в одной строке (включая alignment)
        return static_cast<std::uint32_t>(m_buffer.step[0]);
    }

    //PixelFormat pixelFormat() const noexcept override;

private:
    cv::Mat m_buffer;
};

#endif // CVFRAMEBUFFER_H
