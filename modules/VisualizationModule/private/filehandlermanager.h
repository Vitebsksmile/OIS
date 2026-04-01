#ifndef FILEHANDLERMANAGER_H
#define FILEHANDLERMANAGER_H


#include <QObject>  //  Базовый класс для всех объектов Qt (нужен для сигналов и слотов)
#include <QDebug>   //  Позволяет выводить сообщения в консоль отладки
//#include <QtQml/qqmlregistration.h> //  Макрос для автоматической регистрации класса в системе QML

#include <IVisualizationService.h>
//#include <../private/filehandler.h>


//class FileHandler;  //  Forward declaration

class FileHandlerManager : public QObject
{

    Q_OBJECT

    //QML_ELEMENT


    //Q_PROPERTY(FileHandler *fileHandler READ fileHandler CONSTANT)


    //  Запрещаем QML создавать этот объект напрямую, если он должен жить только внутри C++
    //QML_UNCREATABLE("FileHandlerManager should be accessed via VisualizationService")

    //  Тип будет известен системе метаданных, но не создаваем в QML
    //QML_ANONYMOUS


public:

    explicit FileHandlerManager(IVisualizationService *visualizationService, QObject *parent = nullptr);


    //Q_INVOKABLE void registerFileHandler(FileHandler *fileHandler);


    Q_INVOKABLE void printMessage(const QString &msg);


private:

    FileHandler* createFileHandler();

    IVisualizationService *m_visualizationService = nullptr;

    FileHandler *m_fileHandler = nullptr;
};

#endif // FILEHANDLERMANAGER_H
