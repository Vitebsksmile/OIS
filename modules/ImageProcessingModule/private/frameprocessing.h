#ifndef FRAMEPROCESSING_H
#define FRAMEPROCESSING_H

#include <opencv2/opencv.hpp>

class FrameProcessing
{
public:
    FrameProcessing(cv::Mat cvFrame);

    //~FrameProcessing();

    cv::Mat cvFrame() { return m_frame; }

    FrameProcessing& toGray();
    FrameProcessing& gaussianBlur(int kernelSize = 3);
    FrameProcessing& toBinary();

private:
    //  Ручной сброс ресурсов
    void release();

private:
    cv::Mat m_frame;
};

#endif // FRAMEPROCESSING_H
