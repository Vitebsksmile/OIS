#ifndef IMAGEPREPROCESSING_H
#define IMAGEPREPROCESSING_H


#include <QString>
#include <QDebug>
#include <opencv2/opencv.hpp>

class ImagePreProcessing
{

public:

    //  Конструктор сразу загружает изображение
    explicit ImagePreProcessing(const QString &filePath);

    //  Деструктор
    ~ImagePreProcessing();

    //  Ручной сброс ресурсов
    void release();


    //  Методы обработки (возвращают ссылку на себя для цепочки вызовов)
    ImagePreProcessing& resize(int width, int height);
    ImagePreProcessing& toGray();
    ImagePreProcessing& toRGB();
    ImagePreProcessing& normalize(float alpha = 0, float beta = 1);
    ImagePreProcessing& gaussianBlur(int kernelSize = 3);

    //  Получение результата
    cv::Mat getResult() const;
    bool isValid() const;


private:

    cv::Mat m_image;

};

#endif // IMAGEPREPROCESSING_H
