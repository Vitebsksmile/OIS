#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QObject>
#include <QUrl>
#include <QFile>
#include <QDebug>
#include <QtQml/qqmlregistration.h>

class FileHandler : public QObject
{
    Q_OBJECT
    QML_ELEMENT //  регистрируем для qml

    //  св-во, к-рое qml будет слушать
    Q_PROPERTY(QUrl currentImagePath READ currentImagePath NOTIFY currentImagePathChanged)

public:

    explicit FileHandler(QObject *parent = nullptr);

    //  метод, к-рый вызовем из qml при выборе файла
    Q_INVOKABLE void selectImage(QUrl url);

    QUrl currentImagePath() const;

    //  метод для сохранения (копирования) файла
    Q_INVOKABLE bool saveImage(QUrl sourceUrl, QUrl targetUrl);

    //  метод для подготовки пути
    Q_INVOKABLE QString getCleanPath(QUrl url);

signals:
    void currentImagePathChanged(); //  сигнал для связи с qml

private:
    QUrl m_currentImagePath;    //  храним путь здесь
};

#endif // FILEHANDLER_H
