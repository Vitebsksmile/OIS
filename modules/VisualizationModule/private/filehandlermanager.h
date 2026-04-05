#ifndef FILEHANDLERMANAGER_H
#define FILEHANDLERMANAGER_H


#include <QObject>  //  Базовый класс для всех объектов Qt (нужен для сигналов и слотов)
#include <QtQml/qqmlregistration.h> //  Макрос для автоматической регистрации класса в системе QML

#include <IVisualizationService.h>
#include <filehandler.h>


class FileHandlerManager : public QObject
{

    Q_OBJECT


public:

    explicit FileHandlerManager(IVisualizationService *visualizationService, QObject *parent = nullptr);


    Q_INVOKABLE void registerFileHandler(FileHandler *fileHandler = nullptr);


private:

    IVisualizationService *m_visualizationService = nullptr;

    FileHandler *m_fileHandler = nullptr;
};

#endif // FILEHANDLERMANAGER_H
