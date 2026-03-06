#include "filehandler.h"

FileHandler::FileHandler(QObject *parent) : QObject(parent) {}

void FileHandler::selectImage(QUrl url)
{
    if (m_currentImagePath != url)
    {
        m_currentImagePath = url;
        //  уведомляем qml, что пора перерисовать Image
        emit currentImagePathChanged();
    }
}

QUrl FileHandler::currentImagePath() const
{
    return m_currentImagePath;
}

bool FileHandler::saveImage(QUrl sourceUrl, QUrl targetUrl)
{
    QString sourcePath = sourceUrl.toLocalFile();   //  источник
    QString targetPath = targetUrl.toLocalFile();   //  цель

    //  если файл уже существует (например, пользователь подтвердил замену в диалоге)
    //  QFile::copy не перезаписывает файлы автоматически, поэтому удаляем старый
    if (QFile::exists(targetPath))
    {
        QFile::remove(targetPath);
    }

    if (QFile::copy(sourcePath, targetPath))
    {
        qDebug() << "Файл успешно сохранен в: " << targetPath;
        return true;
    } else {
        qDebug() << "Ошибка при сохранении файла!";
        return false;
    }
}

QString FileHandler::getCleanPath(QUrl url) {
    return url.toLocalFile();
}
