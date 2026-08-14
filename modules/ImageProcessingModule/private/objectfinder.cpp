#include "objectfinder.h"
#include <QDebug>

ObjectFinder::ObjectFinder(cv::Mat cvFrame)
    : m_cvFrame(cvFrame)
{

}

ObjectFinder& ObjectFinder::findObjects()
{
    m_rectanglePoints.clear();
    m_objectsCount = 0;

    if (!m_cvFrame.empty())
    {
        cv::findContours(m_cvFrame, m_contours
                         , cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        for (size_t i = 0; i < m_contours.size(); i++)
        {
            //  Find the bounding box
            cv::Rect boundRect = cv::boundingRect(m_contours[i]);
            if (boundRect.area() > 500)
            {
                std::vector<int> objectCoords { boundRect.x
                                              , boundRect.y
                                              , boundRect.width
                                              , boundRect.height };
                m_rectanglePoints.push_back(objectCoords);

                // m_rectanglePoints[i].push_back(boundRect.x);
                // m_rectanglePoints[i].push_back(boundRect.y);
                // m_rectanglePoints[i].push_back(boundRect.width);
                // m_rectanglePoints[i].push_back(boundRect.height);
            }
        }

        m_objectsCount = m_rectanglePoints.size();


    }
    return *this;
}
