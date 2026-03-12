#include "imageprocessor.h"

ImageProcessor::ImageProcessor() : QQuickImageProvider(Image) {}

QImage ImageProcessor::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    //  id - это путь к файлу, который мы передали из QML
    std::string filePath = id.toStdString();
    cv::Mat mat = cv::imread(filePath);

    if (mat.empty()) return QImage();

    //  --- Предобработка для нейросети ---
    cv::Mat processed;

    //  1. Изменение размера (например, 224х224 для MobileNet/ResNet)
    cv::resize(mat, processed, cv::Size(224, 224));

    //  2. Конвертация в Float (32-bit)
    processed.convertTo(processed, CV_32FC3);

    //  3. Нормализация (например: масшабирование в диапазон [0, 1])
    //  Также здесь можно вычесть среднее (mean) и разделить на std
    cv::normalize(processed, processed, 0, 1.0, cv::NORM_MINMAX);

    //  Для визуализации в QML конвертируем обратно в 8-bit QImage
    //  (Нейросеть бы работала с 'proessed', но экран просит QImage)
    cv::Mat displayMat;
    processed.convertTo(displayMat, CV_8UC3, 255.0);
    cv::cvtColor(displayMat, displayMat, cv::COLOR_BGR2RGB);

    QImage img((const uchar*)displayMat.data, displayMat.cols, displayMat.rows, displayMat.step, QImage::Format_RGB888);
    return img.copy();  //  .copy() выжен, так как Mat локальный
}
