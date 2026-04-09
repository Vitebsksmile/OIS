#include <QDebug>
#include <QStandardPaths>
#include <QDir>
#include <QDateTime>

#include "imagepreprocessing.h"


ImagePreProcessing::ImagePreProcessing()
{

    qDebug() << "ImagePreprocessing: Started preProcessing...";

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
        m_filePath.clear();

    }

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


    qDebug() << "ImagePreProcessing: Путь к файлу получен! Path to image: " << filePath;


    return true;

}


bool ImagePreProcessing::save()
{

    if (m_image.empty()) return "";


    // 1. Получаем путь к папке данных приложения (например, C:/Users/Name/AppData/Roaming/YourApp)
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    // 2. Создаем директорию, если её еще нет
    QDir dir(appDataPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    //  3. Формируем полный путь к файлу
    // Создаем имя на основе времени
    QString finalName = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".jpg";

    QString fullPath = dir.absoluteFilePath(finalName);


    //  4. Подготовка матрицы OpenCV к записи (конвертация float -> 8bit)
    cv::Mat toSave;

    if (m_image.depth() == CV_32F)
    {

        //  Внимание: normalize делает изображение CV_32F (0...1),
        //  imwrite ожидает 0...255 для сохранения в JPG/PNG.
        //  Если была нормализация, то возвращаем в 0...255 для записи в файл
        m_image.convertTo(toSave, CV_8U, 255.0);

    } else {

        toSave = m_image;

    }


    //  5. Запись на диск
    if (cv::imwrite(fullPath.toStdString(), toSave))
    {

        m_finalFilePath = fullPath;
        qDebug() << "Изображение сохранено в файлы приложения: " << fullPath;
        return true;

    } else {

        qCritical() << "Ошибка записи в: " << fullPath;
        return false;

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

    if (!m_image.empty() && kernelSize % 2)
    {

        cv::GaussianBlur(m_image, m_image, cv::Size(kernelSize, kernelSize), 0);

    }

    return *this;

}
