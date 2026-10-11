#include "objectsfinder.h"
#include <QDebug>

ObjectsFinder::ObjectsFinder(cv::Mat cvFrame)
    : m_cvFrame(cvFrame)
{

}

ObjectsFinder& ObjectsFinder::findContours()
{
    m_contours.clear();

    if (!m_cvFrame.empty())
    {
        cv::findContours(m_cvFrame,
                         m_contours,
                         cv::RETR_EXTERNAL,
                         cv::CHAIN_APPROX_SIMPLE);
    }
    return *this;
}

ObjectsFinder &ObjectsFinder::detectPerfectRectangularObjects()
{
    std::vector<cv::Point> approx;

    //  Пребираем контуры и аппроксимируем их полигонами
    for (auto it = m_contours.begin(); it != m_contours.end(); /*---*/) {

        //  Вычисляем периметр и задаем точность аппроксимации (epsilon)
        double perimeter = cv::arcLength(*it, true);
        cv::approxPolyDP(*it, approx, 0.02 * perimeter, true);

        //  Прямоугольник должен иметь 4 вершины, быть замкнутым и иметь достаточную площадь
        if (approx.size() != 4 || !cv::isContourConvex(approx) || cv::contourArea(approx) < m_areaOfObject) {

            it = m_contours.erase(it);
            continue;

        } else {
            //++it;
            //  Проверяем углы (все 4 угла должны быть близки к 90 градусам)
            std::vector<double> coss;
            for (int j = 2; j < 5; j++) {
                double cos = std::fabs(this->angle(approx[j%4], approx[j-2], approx[j-1]));
                coss.push_back(cos);
            }
            //  Косинус 90 градусов равен 0 (или малым значениям)
            for (double cos : coss) {
                //  Погрешность около 17-18 градусов
                if (cos > 0.3) {
                    //  Удаляем контур из m_contours
                    it = m_contours.erase(it);
                    break;
                }
            }

            ++it;
        }
    }

    return *this;
}

ObjectsFinder &ObjectsFinder::detectRaggedRectangularObjects()
{
    //  Находим самый большой контур по площади
    double maxArea = 0;
    for (auto it = m_contours.begin(); it != m_contours.end(); /*---*/) {
        double area = cv::contourArea(*it);
        if (area > m_areaOfObject && area > maxArea) {
            maxArea = area;
            ++it;
        } else {
            it = m_contours.erase(it);
        }
    }

    return *this;
}

ObjectsFinder &ObjectsFinder::makeBoundRect()
{
    m_rectanglePoints.clear();

    for (size_t i = 0; i < m_contours.size(); i++)
    {
        //  Find the bounding box
        cv::Rect boundRect = cv::boundingRect(m_contours[i]);

        std::vector<int> objectCoords { boundRect.x
                                      , boundRect.y
                                      , boundRect.width
                                      , boundRect.height };

        m_rectanglePoints.push_back(objectCoords);
    }

    m_numberOfObjects = m_rectanglePoints.size();

    return *this;
}

// Функция проверки угла между векторами (для угла ~90 градусов)
double ObjectsFinder::angle(const cv::Point &pt1, const cv::Point &pt2, const cv::Point &pt0) {
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1) * (dx2*dx2 + dy2*dy2) + 1e-10);
}
