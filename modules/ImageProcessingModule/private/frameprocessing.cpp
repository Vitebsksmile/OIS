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

FrameProcessing &FrameProcessing::closesGapsInLines()
{
    if (!m_frame.empty())
    {
        cv::Canny(m_frame, m_frame, 50, 150);

        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));

        cv::morphologyEx(m_frame, m_frame, cv::MORPH_CLOSE, kernel);
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

cv::Mat FrameProcessing::cropAndCorrectPCB(const cv::Mat &src,
                                           const std::vector<cv::Point> &pcbContour)
{
    //  Находим минимальный повернутый прямоугольник вокруг контура платы
    cv::RotatedRect rotateRect = cv::minAreaRect(pcbContour);

    //  Получаем угол, центр и размеры
    float angle = rotateRect.angle;
    cv::Size2f rectSize = rotateRect.size;
    cv::Point2f center = rotateRect.center;

    //  Корректируем угол (иногда угол требует нормализации в зависимости от соотношения сторон)
    if (rectSize.width < rectSize.height) {
        std::swap(rectSize.width, rectSize.height);
        angle += 90.0;
    }
    // if (rectSize.width < rectSize.height) {
    //     std::swap(rectSize.width, rectSize.height);
    //     angle += 90.0f;
    // }

    //  Создаем матрицу поворота относительно центра платы
    cv::Mat rotationMatrix = cv::getRotationMatrix2D(center, angle, 1.0);

    //  Поворачиваем все исходное изображение, чтобы плата встала ровно
    cv::Mat rotatedImage;
    cv::warpAffine(src, rotatedImage, rotationMatrix, src.size(), cv::INTER_CUBIC);

    //  Вырезаем (кропаем) плату из уже выровненного изображения
    cv::Mat croppedPCB;
    cv::getRectSubPix(rotatedImage, rectSize, center, croppedPCB);

    return croppedPCB;
}
