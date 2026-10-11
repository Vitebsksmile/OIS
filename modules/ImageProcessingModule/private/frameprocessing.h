#ifndef FRAMEPROCESSING_H
#define FRAMEPROCESSING_H

#include <opencv2/opencv.hpp>
#include "Frame.h"

class FrameProcessing
{
public:
    FrameProcessing(cv::Mat cvFrame);

    //~FrameProcessing();

    cv::Mat cvFrame() { return m_frame; }
    OIS::Core::Frame structFrame() { return m_stFrame; }

    FrameProcessing& toGray();
    FrameProcessing& gaussianBlur(int kernelSize = 3);
    FrameProcessing& closesGapsInLines();
    FrameProcessing& toBinary();

    cv::Mat cropAndCorrectPCB(const cv::Mat &src, const std::vector<cv::Point> &pcbContour);

private:
    //  Ручной сброс ресурсов
    void release();

private:
    cv::Mat m_frame;
    OIS::Core::Frame m_stFrame;
};

#endif // FRAMEPROCESSING_H
