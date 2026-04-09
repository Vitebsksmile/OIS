#ifndef IMAGEPREPROCESSING_H
#define IMAGEPREPROCESSING_H


#include <QString>
#include <memory>
#include <opencv2/opencv.hpp>


class ImagePreProcessing
{

public:

    explicit ImagePreProcessing();

    ~ImagePreProcessing();


    /*  Geter
     *  ???const QString &: Не копируем строку целиком при передаче в функцию. Передаем только адрес, но защищаете данные от изменений.
     *  const в конце геттера: Это помечает метод как «не меняющий состояние объекта». Это позволяет вызывать геттер у константных объектов класса.
    */
    const QString& filePath() const { return QString::fromStdString(m_filePath); }


    const QString& getFinalFilePath() const { return m_finalFilePath; }


    //  Получение результата
    cv::Mat getResult() const { return m_image; }


    //  Проверка изображения по filePath
    bool isValid() const { return !m_image.empty(); }


    //  Ручной сброс ресурсов
    void release();


    bool loadImage(const QString &filePath);


    bool save();


    //  Методы обработки (возвращают ссылку на себя для цепочки вызовов)
    ImagePreProcessing& resize(int width, int height);
    ImagePreProcessing& toGray();
    ImagePreProcessing& toRGB();
    ImagePreProcessing& normalize(float alpha = 0, float beta = 1);
    ImagePreProcessing& gaussianBlur(int kernelSize = 3);


private:

    std::string m_filePath; // ????Автоматически инициализируется пустой строкой ""

    QString m_finalFilePath;


    cv::Mat m_image;

};

#endif // IMAGEPREPROCESSING_H
