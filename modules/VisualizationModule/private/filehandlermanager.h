#ifndef FILEHANDLERMANAGER_H
#define FILEHANDLERMANAGER_H


#include <QObject>
//  Макрос для автоматической регистрации класса в системе QML:
#include <QtQml/qqmlregistration.h>
#include <QQmlEngine>
#include <QList>
#include <QPointer>

#include "IVisualizationService.h"
#include "filehandler.h"
#include "videoProvider.h"


class FileHandlerManager : public QObject
{
    Q_OBJECT
    QML_SINGLETON
    QML_ELEMENT
    // Запрещаем создавать объект этого типа напрямую из QML:
    QML_UNCREATABLE("Interface")

    static FileHandlerManager* s_instance;

public:

    explicit FileHandlerManager(IVisualizationService *visualizationService,
                                QObject *parent = nullptr);

    static FileHandlerManager *create(QQmlEngine *, QJSEngine *)
    { return s_instance; }

    Q_INVOKABLE void registerFileHandler(FileHandler *fileHandler = nullptr);
    Q_INVOKABLE void registerProvider(VideoProvider *provider = nullptr);

private:

    IVisualizationService *m_visualizationService = nullptr;

    QList<QPointer<FileHandler>> m_fileHandlers;
    QList<QPointer<VideoProvider>> m_providers;
};

#endif // FILEHANDLERMANAGER_H
