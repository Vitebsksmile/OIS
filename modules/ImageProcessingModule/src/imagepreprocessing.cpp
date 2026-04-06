#include <QDebug>

#include "imagepreprocessing.h"


ImagePreProcessing::ImagePreProcessing(QObject *parent)
    : QObject(parent)   //  Передаем родителя
{

    qDebug() << "ImagePreprocessing: Started preProcessing...";

}


ImagePreProcessing::~ImagePreProcessing()
{

    release();
    qDebug() << "ImagePreprocessing: объект уничтожен";

}


bool ImagePreProcessing::loadImage(const QString &filePath)
{

    //  Проверяем на пустую строку и преобразуем путь Qt в стандартную строку для OpenCV
    if (filePath.isEmpty()) return false;


    m_filePath = filePath.toStdString();


    m_image = cv::imread(m_filePath);


    if (m_image.empty())
    {

        qWarning() << "Не удалось загрузить изображение по пути: " << filePath;

        return false;

    }


    return true;

}


void ImagePreProcessing::release()
{

    if (!m_image.empty())
    {

        m_image.release();  //  Явное освобождение матрицы OpenCV
        qDebug() << "ImagePreprocessing: память матрицы освобождена";
        m_filePath.clear();

    }

}


ImagePreProcessing& ImagePreProcessing::resize(int width, int height)
{

    if (!m_image.empty())
    {

        cv::resize(m_image, m_image, cv::Size(width, height));

    }

    return *this;

}


ImagePreProcessing& ImagePreProcessing::toGray()
{

    if (!m_image.empty() && m_image.channels() == 3)
    {

        cv::cvtColor(m_image, m_image, cv::COLOR_BGR2GRAY);

    }

    return *this;

}


ImagePreProcessing& ImagePreProcessing::toRGB()
{

    if (!m_image.empty() && m_image.channels() == 3)
    {

        //  OpenCV читает BGR, нейронки часто ждут RGB
        cv::cvtColor(m_image, m_image, cv::COLOR_BGR2RGB);

    }

    if (!m_image.empty() && m_image.channels() == 1)
    {

        //  OpenCV читает BGR, нейронки часто ждут RGB
        cv::cvtColor(m_image, m_image, cv::COLOR_GRAY2RGB);

    }

    return *this;

}


ImagePreProcessing& ImagePreProcessing::normalize(float alpha, float beta)
{

    if (!m_image.empty())
    {

        //  Приведение значений пикселей к диапазону [0, 1] или другому
        // Конвертируем в float (32-bit), если планируем нормализацию 0..1
        m_image.convertTo(m_image, CV_32F, 1.0 / 255.0);

        // ВЫЗЫВАЕМ КАК СВОБОДНУЮ ФУНКЦИЮ:
        cv::normalize(m_image, m_image, alpha, beta, cv::NORM_MINMAX);

    }

    return *this;

}


ImagePreProcessing& ImagePreProcessing::gaussianBlur(int kernelSize)
{

    if (!m_image.empty() && !(kernelSize % 2))
    {

        cv::GaussianBlur(m_image, m_image, cv::Size(kernelSize, kernelSize), 0);

    }

    return *this;

}
