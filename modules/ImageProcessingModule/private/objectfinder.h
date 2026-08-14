#ifndef OBJECTFINDER_H
#define OBJECTFINDER_H

//#include <vector>
#include <opencv2/opencv.hpp>

class ObjectFinder
{
public:
    ObjectFinder(cv::Mat cvFrame);

    //~ObjectFinder();

    size_t objectCount() { return m_objectsCount; }
    std::vector<std::vector<int>> rectanglePoints() { return m_rectanglePoints; }

    //  Method for finding object bounding rectangle points
    ObjectFinder& findObjects();

private:
    cv::Mat m_cvFrame;

    //  Contours list
    std::vector<std::vector<cv::Point>> m_contours;

    //  Object count
    size_t m_objectsCount;

    //  List of rectangle points
    std::vector<std::vector<int>> m_rectanglePoints;
};

#endif // OBJECTFINDER_H
