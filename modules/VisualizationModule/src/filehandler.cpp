/*
 *  Сигналы (signals): Только объявляем в .h, никогда не пишем тело в .cpp.
 *  Слоты (public slots): Объявляем в .h и обязательно пишем логику в .cpp.
 *  Методы (Q_INVOKABLE): Объявляем в .h и обязательно пишем логику в .cpp.
*/


#include <QDebug>   //  Позволяет выводить сообщения в консоль отладки

#include "filehandler.h"


//  Конструктор: инициализирует объект и передает родителя для управления памятью в Qt
FileHandler::FileHandler(QObject *parent)
    : QObject(parent)
{
}


//  Геттер: возвращает текущий путь в формате QUrl для отображения в интерфейсе
QUrl FileHandler::currentImagePath() const
{

    return m_currentImagePath;

}


void FileHandler::setCurrentImagePath(QUrl url)
{

    m_currentImagePath = url;

}


//  Установка направления (чтение/отправка)
void FileHandler::setDirectionOut(bool out)
{

    if (m_directionOut != out)
    {

        m_directionOut = out;

        emit directionOutChanged();

    }

}


//  Вспомогательная функция для получения "чистого" пути
//  Нужна для OpenCV, так как cv::imread не понимает префикс "file://"
QString FileHandler::getCleanPath(QUrl url) {
    //  Самый надежный способ конвертации URL в путь ОС в Qt
    return url.toLocalFile();
}


//  Метод возвращает размер файла в байтах
double FileHandler::getFileSize(const QUrl &url)
{

    QString filePath = getCleanPath(url);

    QFileInfo fi(filePath);

    return fi.size() / (1024.0 * 1024.0);

}


//  Метод для выбора изображения (вызывается из QML при выборе файла)
void FileHandler::selectImage(QUrl url)
{
    if (currentImagePath() != url)
    {

        m_currentImagePath = url;


        //  уведомляем qml, что пора перерисовать Image
        emit currentImagePathChanged();

    }
}


//  Метод для копирования/сохранения изображения
//  или через диалоговое окно для пользователя
bool FileHandler::saveImage(QUrl sourceUrl, QUrl targetUrl)
{

    QString sourcePath = sourceUrl.toLocalFile();   //  источник
    QString targetPath = targetUrl.toLocalFile();   //  цель


    //  Если файл уже существует (например, пользователь подтвердил замену в диалоге)
    //  QFile::copy не перезаписывает файлы автоматически, поэтому удаляем старый
    if (QFile::exists(targetPath))
    {

        //  Если файл есть, удаляем его, чтобы перезаписать новой версией
        QFile::remove(targetPath);

    }


    //  Выполняем копирование файла средствами операционной системы
    if (QFile::copy(sourcePath, targetPath))
    {

        //  Вывод в консоль отладки при успехе
        qDebug() << "FileHandler: The file was successfully saved to: " << targetPath;

        m_currentImagePath = targetUrl;

        //  уведомляем qml, что пора перерисовать Image
        emit currentImagePathChanged();

        return true;

    } else {

        //  Вывод в консоль при ошибке (например, нет прав доступа или диск переполнен)
        qDebug() << "Error saving file!";
        return false;

    }

}


//  Метод для старта предобработки
//  Срабатывает, когда user нажимает кнопку 'SrartPreprocessing'
void FileHandler::startPreprocessing()
{

    if (!currentImagePath().isEmpty())
    {

        QString filePath = getCleanPath(currentImagePath());

        //  Активируем сигнал
        emit imagePreProcessingRequested(filePath);

        qDebug() << "FileHandler: The image was transferred to the interface via the path: " << currentImagePath();

    } else {

        qDebug() << "FileHandler: ERROR! The Start command was not sent to the interface. No file was selected. Path: " << currentImagePath();

    }

}


void FileHandler::onImagePreProcessingFinished(const QString &filePath)
{

    qDebug() << "FileHandler: The path to the preprocessed file has been obtained. filePath: " << filePath;

    //???  уведомляем qml, что пора перерисовать Image
    selectImage(filePath);

}
