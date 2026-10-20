#ifndef FILEHANDLERCONTROLLER_H
#define FILEHANDLERCONTROLLER_H

#include <QObject>
//  Макрос для автоматической регистрации класса в системе QML:
#include <QtQml/qqmlregistration.h>
#include <QQmlEngine>
#include <QList>
#include <QPointer>
#include "VisualizationService.h"
#include "filehandler.h"
#include <QAbstractItemModel>
#include <QSharedPointer>

class FileHandlerController : public QObject
{
    Q_OBJECT
    QML_SINGLETON
    QML_ELEMENT
    // Запрещаем создавать объект этого типа напрямую из QML:
    QML_UNCREATABLE("Controllers")

    static FileHandlerController* s_instance;

public:
    explicit FileHandlerController(VisualizationService* service,
                                QObject* parent = nullptr);

    static FileHandlerController* create(QQmlEngine *, QJSEngine *) { return s_instance; }

    Q_INVOKABLE void registerFileHandler(FileHandler* fileHandler = nullptr);

signals:
    void operatorsModelChanged();

private:
    VisualizationService* m_service = nullptr;

    QList<QPointer<FileHandler>> m_fileHandlers;
};

#endif // FILEHANDLERCONTROLLER_H
