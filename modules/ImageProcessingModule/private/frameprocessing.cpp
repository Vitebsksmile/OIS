#include "frameprocessing.h"
#include <QDebug>

FrameProcessing::FrameProcessing(cv::Mat cvFrame)
    : m_frame(cvFrame)
{

}

// FrameProcessing::~FrameProcessing()
// {
//     release();
//     qDebug() << "FrameProcessing: Object destroyed";
// }

// void FrameProcessing::release()
// {
//     if (!m_frame->empty())
//     {
//         m_frame->release();  //  Явное освобождение матрицы OpenCV
//         qDebug() << "FrameProcessing: Matrix memory freed";
//     }
// }

FrameProcessing& FrameProcessing::toGray()
{
    if (!m_frame.empty() && m_frame.channels() == 3)
    {
        cv::cvtColor(m_frame, m_frame, cv::COLOR_BGR2GRAY);
    }
    return *this;
}

FrameProcessing& FrameProcessing::gaussianBlur(int kernelSize)
{
    if (!m_frame.empty() && kernelSize % 2)
    {
        cv::GaussianBlur(m_frame, m_frame, cv::Size(kernelSize, kernelSize), 0);
    }

    return *this;
}

FrameProcessing& FrameProcessing::toBinary()
{
    if (!m_frame.empty())
    {
        cv::threshold(m_frame, m_frame, 100, 255, cv::THRESH_BINARY);
    }
    return *this;
}
