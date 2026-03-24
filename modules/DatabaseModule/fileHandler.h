#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QObject>  //  Базовый класс для всех объектов Qt (нужен для сигналов и слотов)
#include <QUrl>     //  Класс для работы с URL (удобен для QML, так как пути там — это URL)
#include <QFile>    //  Класс для работы с файловой системой (чтение, запись, копирование)
#include <QDebug>   //  Позволяет выводить сообщения в консоль отладки
//#include <QtQml/qqmlregistration.h> //  Макрос для автоматической регистрации класса в системе QML

#include "../VisualizationModule/filehandler.h"


//  Наследуемся от QObject, чтобы использовать Qt (MOC), обеспечивая поддержку рефлексии,
//  механизма сигналов и слотов, а также динамических свойств, не поддерживаемых стандартным C++
class FileHandler : public QObject
{
    //  Обязательный макрос для любого класса, использующего сигналы/слоты (свойства)
    Q_OBJECT

    //   Делает класс доступным в Qml (можно будет написать "DatabaseHabdler {}" в .qml)
    //QML_ELEMENT //  регистрируем для Qml

    //  Связываем C++ и Qml: св-во, к-рое Qml будет слушать
    Q_PROPERTY(QUrl currentImagePath READ currentImagePath NOTIFY currentImagePathChanged)
    //  currentImagePath - name of the property in Qml
    //  READ ... - the function that gives meaning
    //  NOTIFY ... - signal that tells Qml: "meaning has changed, redraw the interface"
    //  currentImagePathChanged - this is the "voice" of your C++ class, which tells the grafical
    //  interface (Qml): "The data has changed, update the image!"

    QUrl currentImagePath() const;

public:
    explicit DatabaseHandler(QObject *parent = nullptr);

signals:
    void currentImagePathChanged();


};

#endif // FILEHANDLER_H
