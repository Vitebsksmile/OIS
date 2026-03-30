#ifndef FILEHANDLERMANAGER_H
#define FILEHANDLERMANAGER_H


#include <QObject>  //  Базовый класс для всех объектов Qt (нужен для сигналов и слотов)
#include <QDebug>   //  Позволяет выводить сообщения в консоль отладки
#include <QtQml/qqmlregistration.h> //  Макрос для автоматической регистрации класса в системе QML

#include <IVisualizationService.h>


class FileHandlerManager : public QObject
{

    Q_OBJECT


    QML_ELEMENT
    // Запрещаем QML создавать этот объект напрямую, если он должен жить только внутри сервиса
    QML_UNCREATABLE("FileHandlerManager should be accessed via VisualizationService")


public:

    explicit FileHandlerManager(IVisualizationService *service, QObject *parent = nullptr);


    //  Метод для Qml: создает FileHandler и сразу связывает его с VisualizationService
    Q_INVOKABLE FileHandler* createFileHandler();


private:

    IVisualizationService *m_service = nullptr;
};

#endif // FILEHANDLERMANAGER_H
