#ifndef FILEHANDLERMANAGER_H
#define FILEHANDLERMANAGER_H


#include <QObject>  //  Базовый класс для всех объектов Qt (нужен для сигналов и слотов)
//#include <QtQml/qqmlregistration.h> //  Макрос для автоматической регистрации класса в системе QML
#include <QList>
#include <QPointer>

#include <IVisualizationService.h>
#include <filehandler.h>


class FileHandlerManager : public QObject
{

    Q_OBJECT

    //QML_ELEMENT

    //QML_UNCREATABLE("Interface") // Запрещает создавать объект этого типа напрямую из QML


public:

    explicit FileHandlerManager(IVisualizationService *visualizationService, QObject *parent = nullptr);


    Q_INVOKABLE void registerFileHandler(FileHandler *fileHandler = nullptr);


private:

    IVisualizationService *m_visualizationService = nullptr;

    QList<QPointer<FileHandler>> m_fileHandlers;
};

#endif // FILEHANDLERMANAGER_H
