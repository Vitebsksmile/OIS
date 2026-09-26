#ifndef DBMODELCONTROLLER_H
#define DBMODELCONTROLLER_H

#include <QObject>
#include <QtQml/qqmlregistration.h>
#include <QQmlEngine>
#include <QSharedPointer>
#include <QAbstractItemModel>
#include "VisualizationService.h"
#include "IDbModel.h"
#include "IDatabaseService.h"

class DbModelController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    // Запрещаем создавать объект этого типа напрямую из QML:
    QML_UNCREATABLE("Controllers")

    static DbModelController *s_instance;

public:
    explicit DbModelController(VisualizationService *visualization,
                               QObject *parent = nullptr);

    static DbModelController* create(QQmlEngine *, QJSEngine *) { return s_instance; }

    Q_INVOKABLE QAbstractTableModel* abstractTableModel(const QString &tableName);
    Q_INVOKABLE void logAbstractModel(QAbstractItemModel *model);
    Q_INVOKABLE void logQmlModelRoles(QAbstractItemModel *model);

private:
    VisualizationService *m_visualization = nullptr;
    IDatabaseService *m_dbService = nullptr;

    QHash<QString, QPointer<QAbstractTableModel>> m_abstractModelsMap{};
};

#endif // DBMODELCONTROLLER_H
