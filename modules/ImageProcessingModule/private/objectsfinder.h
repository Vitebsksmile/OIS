#ifndef OBJECTSFINDER_H
#define OBJECTSFINDER_H

#include <opencv2/opencv.hpp>

class ObjectsFinder
{
public:
    ObjectsFinder(cv::Mat cvFrame);

    //~ObjectsFinder();

    size_t numberOfObjects() { return m_numberOfObjects; }
    std::vector<std::vector<int>> rectanglePoints() { return m_rectanglePoints; }
    std::vector<std::vector<cv::Point>> contours() { return m_contours; }

    //  Method for finding object bounding rectangle points
    ObjectsFinder& findContours();

    ObjectsFinder& detectPerfectRectangularObjects();
    ObjectsFinder& detectRaggedRectangularObjects();
    ObjectsFinder& makeBoundRect();

private:
    double angle(const cv::Point &pt1, const cv::Point &pt2, const cv::Point &pt0);

    int m_areaOfObject = 50000;

    cv::Mat m_cvFrame;

    //  Contours list
    std::vector<std::vector<cv::Point>> m_contours;

    //  Object count
    size_t m_numberOfObjects = 0;

    //  List of rectangle points
    std::vector<std::vector<int>> m_rectanglePoints;
};

#endif // OBJECTSFINDER_H
