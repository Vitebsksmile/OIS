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

    //  Получаем 4 угла контура платы
    //  Корректируем угол (иногда угол требует нормализации в зависимости от соотношения сторон)
    if (rectSize.width < rectSize.height) {
        std::swap(rectSize.width, rectSize.height);
        angle += 90.0;
    }

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

cv::Mat FrameProcessing::cropAndCorrectPCB_2(const cv::Mat &src,
                                             const std::vector<cv::Point> &pcbContour)
{
    //  Находим минимальный повернутый прямоугольник вокруг контура платы
    cv::RotatedRect rotateRect = cv::minAreaRect(pcbContour);

    //  Получаем угол, центр и размеры
    float angle = rotateRect.angle;
    cv::Size2f rectSize = rotateRect.size;
    cv::Point2f center = rotateRect.center;

    //  Получаем 4 угла контура платы
    //  Корректируем угол (иногда угол требует нормализации в зависимости от соотношения сторон)
    if (rectSize.width < rectSize.height) {
        std::swap(rectSize.width, rectSize.height);
        angle += 90.0;
    }

    //  Создаем матрицу поворота относительно центра платы
    //cv::Mat rotationMatrix = cv::getRotationMatrix2D(center, angle, 1.0);

    //  Поворачиваем все исходное изображение, чтобы плата встала ровно
    //cv::Mat rotatedImage;
    //cv::warpAffine(src, rotatedImage, rotationMatrix, src.size(), cv::INTER_CUBIC);

    //  Вырезаем (кропаем) плату из уже выровненного изображения
    cv::Mat croppedPCB;
    //cv::getRectSubPix(rotatedImage, rectSize, center, croppedPCB);
    cv::getRectSubPix(src, rectSize, center, croppedPCB);

    return croppedPCB;
}

cv::Mat FrameProcessing::cropAndCorrectPCB_3(const cv::Mat &src,
                                             const std::vector<cv::Point> &pcbContour)
{
    //  Находим минимальный повернутый прямоугольник вокруг контура платы
    cv::RotatedRect rotateRect = cv::minAreaRect(pcbContour);

    //  Получаем 4 угла контура платы
    cv::Point2f vertices[4];
    rotateRect.points(vertices);

    //  Считаем реальные физические длины сторон (по теореме Пифагора)
    double edge1 = cv::norm(vertices[0] - vertices[1]);
    double edge2 = cv::norm(vertices[1] - vertices[2]);

    //  Опрелеляем, где реальная ширина, а где высота
    float width = (edge1 > edge2) ? edge1 : edge2;
    float height = (edge1 > edge2) ? edge2 : edge1;

    //  Округляем до целых пикселей для целевого изображения
    int targetWidth = cvRound(width);
    int targetHeight = cvRound(height);

    //  Задаем координаты углов на целевом (выровненном) изображении
    std::vector<cv::Point2f> dstPoints = {
        cv::Point2f(0, targetHeight - 1),
        cv::Point2f(0, 0),
        cv::Point2f(targetWidth - 1, 0),
        cv::Point2f(targetWidth - 1, targetHeight - 1)
    };

    // return croppedPCB;

    //  Сортируем исходные вершины контура, чтобы они соответствовали dstPoints
    //  В OpenCV rotatedRect.points() всегда возвращает точки в определенном порядке (снизу-вверх, слева-направо)
    std::vector<cv::Point2f> srcPoints(4);
    if (edge1 > edge2) {
        srcPoints[0] = vertices[0];
        srcPoints[1] = vertices[1];
        srcPoints[2] = vertices[2];
        srcPoints[3] = vertices[3];
    } else {
        srcPoints[0] = vertices[1];
        srcPoints[1] = vertices[2];
        srcPoints[2] = vertices[3];
        srcPoints[3] = vertices[0];
    }

    //  Вычисляем матрицу трансформации и проецируем пиксели 1-к-1
    cv::Mat transformMatrix = cv::getPerspectiveTransform(srcPoints, dstPoints);

    cv::Mat croppedPCB;
    cv::warpPerspective(src, croppedPCB, transformMatrix, cv::Size(targetWidth, targetHeight), cv::INTER_LANCZOS4);

    //  Клонируем матрицу, чтобы гарантированно сбросить нестандартный stride/step в памяти
    return croppedPCB.clone();
}

cv::Mat FrameProcessing::cropAndCorrectPCB_4(cv::Mat src,
                                             std::vector<cv::Point> pcbContour)
{
    if (pcbContour.empty() || src.empty()) return cv::Mat();

    // 1. Получаем повернутый прямоугольник платы и его 4 вершины
    cv::RotatedRect rotatedRect = cv::minAreaRect(pcbContour);
    cv::Point2f pts[4];
    rotatedRect.points(pts);

    // 2. ГЕОМЕТРИЧЕСКАЯ СОРТИРОВКА ТОЧЕК (не зависит от углов OpenCV)
    std::vector<cv::Point2f> srcPoints(pts, pts + 4);

    // Сортируем точки по координате X (слева направо)
    std::sort(srcPoints.begin(), srcPoints.end(), [](const cv::Point2f& a, const cv::Point2f& b) {
        return a.x < b.x;
    });

    // Теперь srcPoints[0] и [1] — левые точки, srcPoints[2] и [3] — правые точки.
    cv::Point2f topLeft = (srcPoints[0].y < srcPoints[1].y) ? srcPoints[0] : srcPoints[1];
    cv::Point2f bottomLeft = (srcPoints[0].y < srcPoints[1].y) ? srcPoints[1] : srcPoints[0];
    cv::Point2f topRight = (srcPoints[2].y < srcPoints[3].y) ? srcPoints[2] : srcPoints[3];
    cv::Point2f bottomRight = (srcPoints[2].y < srcPoints[3].y) ? srcPoints[3] : srcPoints[2];

    // Упаковываем в строгом порядке: ЛВ, ПВ, ПН, ЛН
    std::vector<cv::Point2f> orderedSrc = { topLeft, topRight, bottomRight, bottomLeft };

    // 3. Вычисляем РЕАЛЬНЫЕ физические размеры сторон платы (длина векторов)
    double widthTop = cv::norm(topRight - topLeft);
    double widthBottom = cv::norm(bottomRight - bottomLeft);
    double heightLeft = cv::norm(bottomLeft - topLeft);
    double heightRight = cv::norm(bottomRight - topRight);

    // Берем максимальные значения, чтобы разъемы на краях платы не обрезались
    int targetWidth = cvRound(std::max(widthTop, widthBottom));
    int targetHeight = cvRound(std::max(heightLeft, heightRight));

    if (targetWidth <= 0 || targetHeight <= 0) return cv::Mat();

    // 4. Задаем опорные точки целевого изображения (куда проецируем)
    std::vector<cv::Point2f> orderedDst = {
        cv::Point2f(0, 0),
        cv::Point2f(targetWidth - 1, 0),
        cv::Point2f(targetWidth - 1, targetHeight - 1),
        cv::Point2f(0, targetHeight - 1)
    };

    // 5. Вычисляем матрицу трансформации и проецируем пиксели 1-к-1
    cv::Mat transformMatrix = cv::getPerspectiveTransform(orderedSrc, orderedDst);

    cv::Mat croppedPCB;
    cv::warpPerspective(src, croppedPCB, transformMatrix, cv::Size(targetWidth, targetHeight), cv::INTER_LANCZOS4);

    // 6. Полный сброс структуры памяти
    return croppedPCB.clone();
}

cv::Mat FrameProcessing::cropAndCorrectPCB_5(const cv::Mat &src, const std::vector<cv::Point> &pcbContour)
{
    if (src.empty() || pcbContour.size() < 4) {
        return {};
    }

    // 1. Находим минимальный повернутый прямоугольник.
    const cv::RotatedRect rotatedRect =
        cv::minAreaRect(pcbContour);

    if (rotatedRect.size.width < 2.0f ||
        rotatedRect.size.height < 2.0f) {
        return {};
    }

    // 2. Получаем четыре вершины.
    cv::Point2f vertices[4];
    rotatedRect.points(vertices);

    std::vector<cv::Point2f> points(vertices, vertices + 4);

    // 3. Сортируем вершины по углу относительно центра.
    // В координатах изображения возрастающий atan2 даёт
    // обход вершин в порядке TL, TR, BR, BL после нормализации.
    const cv::Point2f center = rotatedRect.center;

    std::sort(
        points.begin(),
        points.end(),
        [&center](const cv::Point2f &a,
                  const cv::Point2f &b)
        {
            const double angleA = std::atan2(
                a.y - center.y, a.x - center.x);

            const double angleB = std::atan2(
                b.y - center.y, b.x - center.x);

            return angleA < angleB;
        });

    // 4. Переносим начальную точку на вершину,
    // ближайшую к верхнему левому углу изображения.
    const auto topLeftIt = std::min_element(
        points.begin(),
        points.end(),
        [](const cv::Point2f &a,
           const cv::Point2f &b)
        {
            const float sumA = a.x + a.y;
            const float sumB = b.x + b.y;

            if (sumA != sumB) {
                return sumA < sumB;
            }

            return a.y < b.y;
        });

    std::rotate(points.begin(), topLeftIt, points.end());

    // 5. Проверяем направление обхода.
    // Для TL -> TR -> BR -> BL signed area положительна
    // в стандартной системе координат изображения.
    double signedArea = 0.0;

    for (int i = 0; i < 4; ++i) {
        const cv::Point2f &a = points[i];
        const cv::Point2f &b = points[(i + 1) % 4];

        signedArea +=
            static_cast<double>(a.x) * b.y -
            static_cast<double>(b.x) * a.y;
    }

    if (std::abs(signedArea) < 1.0) {
        return {};
    }

    if (signedArea < 0.0) {
        // Сохраняем первую вершину, меняем направление обхода.
        std::reverse(points.begin() + 1, points.end());
    }

    // 6. Рассчитываем размеры по противоположным сторонам.
    const double widthTop =
        cv::norm(points[1] - points[0]);

    const double widthBottom =
        cv::norm(points[2] - points[3]);

    const double heightRight =
        cv::norm(points[2] - points[1]);

    const double heightLeft =
        cv::norm(points[3] - points[0]);

    const int targetWidth =
        cvRound(std::max(widthTop, widthBottom));

    const int targetHeight =
        cvRound(std::max(heightLeft, heightRight));

    if (targetWidth < 2 || targetHeight < 2) {
        return {};
    }

    // 7. Углы целевого изображения в том же порядке.
    const std::vector<cv::Point2f> destination = {
        {0.0f, 0.0f},
        {static_cast<float>(targetWidth - 1), 0.0f},
        {static_cast<float>(targetWidth - 1),
         static_cast<float>(targetHeight - 1)},
        {0.0f, static_cast<float>(targetHeight - 1)}
    };

    // 8. Проверяем, что исходные углы находятся внутри кадра.
    // Если прямоугольник выходит за границы, warpPerspective
    // заполнит недостающую область чёрным цветом.
    for (const cv::Point2f &point : points) {
        if (!std::isfinite(point.x) ||
            !std::isfinite(point.y)) {
            return {};
        }
    }

    const cv::Mat transform = cv::getPerspectiveTransform(
        points.data(), destination.data());

    cv::Mat result;

    cv::warpPerspective(
        src,
        result,
        transform,
        cv::Size(targetWidth, targetHeight),
        cv::INTER_CUBIC,
        cv::BORDER_REPLICATE);

    return result;
}
