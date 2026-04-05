#include "imagepreprocessing.h"

ImagePreProcessing::ImagePreProcessing()
{

    //  Перенести в какой-нибудь метод
    //  Преобразуем путь Qt в стандартную строку для OpenCV
    //std::string path = filePath.toStdString();
    //m_image = cv::imread(path);

    qDebug() << "ImagePreprocessing: объект рожден";

    /*if (m_image.empty())
    {

        qWarning() << "Не удалось загрузить изображение по пути: " << filePath;

    }*/

}


ImagePreProcessing::~ImagePreProcessing()
{

    release();
    qDebug() << "ImagePreprocessing: объект уничтожен";

}


void ImagePreProcessing::release()
{

    if (!m_image.empty())
    {

        m_image.release();  //  Явное освобождение матрицы OpenCV
        qDebug() << "ImagePreprocessing: память матрицы освобождена";

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

    if (!m_image.empty())
    {

        cv::GaussianBlur(m_image, m_image, cv::Size(kernelSize, kernelSize), 0);

    }

    return *this;

}


cv::Mat ImagePreProcessing::getResult() const
{

    return m_image;

}


bool ImagePreProcessing::isValid() const
{

    return !m_image.empty();

}


//  Слушает сигнал из ProcessManager
void ImagePreProcessing::onImagePreProcessingRequested(const QString &filePath)
{}
