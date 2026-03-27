#include "imagepreprocessing.h"

ImagePreProcessing::ImagePreProcessing() : QQuickImageProvider(Image) {}

QImage ImagePreProcessing::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
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

    // Конвертируем из BGR (OpenCV) в RGB (Qt)
    cv::cvtColor(displayMat, displayMat, cv::COLOR_BGR2RGB);

    /*  Эта строка — «мостик», который позволяет Qt отобразить данные, обработанные в OpenCV,
     *  без копирования их в новую память. Вы создаете объект QImage, который просто «смотрит»
     *  на ту же область памяти, где лежат пиксели cv::Mat. */
    //  (const uchar*)displayMat.data - Прямой адрес в оперативной памяти, где начинается первый пиксель вашей матрицы
    //  OpenCV хранит данные как uchar*, а Qt ожидает const uchar*. Мы просто подтверждаем, что Qt не будет менять эти данные
    //  displayMat.cols и displayMat.rows - Ширина и высота
    //  displayMat.step - Шаг строки. Step говорит Qt: «Через сколько байт в памяти начинается следующая строка пикселей»
    //  QImage::Format_RGB888 - Формат цвета. RGB888 означает, что на каждый пиксель идет 3 байта (Red, Green, Blue) по 8 бит каждый
    QImage img((const uchar*)displayMat.data, displayMat.cols, displayMat.rows, displayMat.step, QImage::Format_RGB888);

    return img.copy();  //  .copy() выжен, так как Mat локальный
}
