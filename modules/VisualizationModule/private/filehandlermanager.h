#ifndef FILEHANDLERMANAGER_H
#define FILEHANDLERMANAGER_H


#include <QObject>  //  Базовый класс для всех объектов Qt (нужен для сигналов и слотов)
#include <QtQml/qqmlregistration.h> //  Макрос для автоматической регистрации класса в системе QML
#include <QQmlEngine>
#include <QList>
#include <QPointer>

#include <IVisualizationService.h>
#include <filehandler.h>


class FileHandlerManager : public QObject
{

    Q_OBJECT

    QML_SINGLETON

    QML_ELEMENT

    QML_UNCREATABLE("Interface") // Запрещает создавать объект этого типа напрямую из QML

    static FileHandlerManager* s_instance;


public:

    explicit FileHandlerManager(IVisualizationService *visualizationService, QObject *parent = nullptr);
    //FileHandlerManager(IVisualizationService *visualizationService, QObject *parent = nullptr);


    static FileHandlerManager *create(QQmlEngine *, QJSEngine *) { return s_instance; }


    Q_INVOKABLE void registerFileHandler(FileHandler *fileHandler = nullptr);


private:

    IVisualizationService *m_visualizationService = nullptr;

    QList<QPointer<FileHandler>> m_fileHandlers;
};

#endif // FILEHANDLERMANAGER_H
