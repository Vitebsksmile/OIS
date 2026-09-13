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
#include <QAbstractItemModel>
#include <QSharedPointer>
#include "IDbModel.h"

class FileHandlerManager : public QObject
{
    Q_OBJECT
    QML_SINGLETON
    QML_ELEMENT
    // Запрещаем создавать объект этого типа напрямую из QML:
    QML_UNCREATABLE("Interface")

    static FileHandlerManager* s_instance;

public:
    explicit FileHandlerManager(IVisualizationService* visualization,
                                QObject* parent = nullptr);

    static FileHandlerManager* create(QQmlEngine *, QJSEngine *) { return s_instance; }

    Q_INVOKABLE void registerFileHandler(FileHandler* fileHandler = nullptr);

signals:
    void operatorsModelChanged();

private:
    IVisualizationService* m_visualization = nullptr;

    QList<QPointer<FileHandler>> m_fileHandlers;

    QSharedPointer<IDbModel> m_dbModel;
};

#endif // FILEHANDLERMANAGER_H
