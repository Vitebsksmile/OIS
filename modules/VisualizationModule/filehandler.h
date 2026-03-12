#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QObject>  //  Базовый класс для всех объектов Qt (нужен для сигналов и слотов)
#include <QUrl>     //  Класс для работы с URL (удобен для QML, так как пути там — это URL)
#include <QFile>    //  Класс для работы с файловой системой (чтение, запись, копирование)
#include <QDebug>   //  Позволяет выводить сообщения в консоль отладки
#include <QtQml/qqmlregistration.h> //  Макрос для автоматической регистрации класса в системе QML

// Наследуемся от QObject, чтобы использовать мощь Qt (MOC)
class FileHandler : public QObject
{
    //  Обязательный макрос для любого класса, использующего сигналы/свойства
    Q_OBJECT

    //  Делает класс доступным в QML (вы сможете написать "FileHandler {}" в .qml)
    QML_ELEMENT //  регистрируем для qml

    //  Связываем C++ и QML:
    //  св-во, к-рое qml будет слушать
    Q_PROPERTY(QUrl currentImagePath READ currentImagePath NOTIFY currentImagePathChanged)
    //  currentImagePath — имя свойства в QML
    //  READ ... — функция, которая отдает значение
    //  NOTIFY ... — сигнал, который говорит QML "значение изменилось, перерисуй интерфейс"
    //  currentImagePathChanged — это «голос» вашего C++ класса, который сообщает графическому интерфейсу (QML): «Данные изменились, обнови картинку!»

public:

    //  Конструктор. explicit запрещает неявное преобразование типов.
    //  parent = nullptr позволяет объекту существовать без родителя.
    explicit FileHandler(QObject *parent = nullptr);

    //  метод, к-рый вызовем из qml при выборе файла
    Q_INVOKABLE void selectImage(QUrl url);

    //  Геттер для свойства currentImagePath
    QUrl currentImagePath() const;

    //  метод для сохранения (копирования) файла
    Q_INVOKABLE bool saveImage(QUrl sourceUrl, QUrl targetUrl);

    //  метод для подготовки пути
    Q_INVOKABLE QString getCleanPath(QUrl url);

signals:

    //  Сигнал: мы не пишем его реализацию, Qt сделает это за нас
    //  Вызываем его через emit, когда m_currentImagePath меняется
    void currentImagePathChanged();

private:

    //  Внутренняя переменная, где реально хранится путь к файлу
    QUrl m_currentImagePath;    //  храним путь здесь
};

#endif // FILEHANDLER_H
