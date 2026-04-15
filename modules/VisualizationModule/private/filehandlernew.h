#ifndef FILEHANDLERNEW_H
#define FILEHANDLERNEW_H


#include <QObject>                  //  Базовый класс для всех объектов Qt (нужен для сигналов и слотов)
#include <QtQml/qqmlregistration.h> //  Макрос для автоматической регистрации класса в системе QML
//#include <QtQml/qqml.h>
#include <QFile>                    //  Класс для работы с файловой системой (чтение, запись, копирование)
#include <QUrl>                     //  Класс для работы с URL (удобен для QML, так как пути там — это URL)


class FileHandlerNew : public QObject
{

    //  Обязательный макрос для любого класса, использующего сигналы/слоты (свойства)
    Q_OBJECT


    //  Делает класс доступным в Qml (можно будет написать "FileHandler {}" в .qml)
    QML_ELEMENT


public:
    FileHandlerNew();
};

#endif // FILEHANDLERNEW_H
