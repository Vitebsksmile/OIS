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
    //Q_PROPERTY(QAbstractItemModel *operatorsModel READ operatorsModel NOTIFY operatorsModelChanged)
    //Q_PROPERTY(QAbstractItemModel* currentModel READ currentModel NOTIFY currentModelChanged)
    QML_ELEMENT
    QML_SINGLETON
    // Запрещаем создавать объект этого типа напрямую из QML:
    QML_UNCREATABLE("Controllers")

    static DbModelController *s_instance;

public:
    explicit DbModelController(VisualizationService *visualization,
                               QObject *parent = nullptr);

    static DbModelController* create(QQmlEngine *, QJSEngine *) { return s_instance; }

    void setDbService(IDatabaseService *dbService);

    void updateModel(IDbModel *dbModel);

    Q_INVOKABLE QAbstractItemModel* dbModel(const QString &tableName) const;
    Q_INVOKABLE QAbstractTableModel* abstractTableModel(const QString &tableName);
    Q_INVOKABLE void logAbstractModel(QAbstractItemModel *model);
    Q_INVOKABLE void logQmlModelRoles(QAbstractItemModel *model);
    //Q_INVOKABLE void save() { m_dbModel->saveChanges(); }

    // OperatorsController(const OperatorsController& ) = delete;
    // OperatorsController& operator=(const OperatorsController&) = delete;

signals:
    void operatorsModelChanged();
    void currentModelChanged();

private:
    void populateModelsMap();

    VisualizationService *m_visualization = nullptr;
    IDatabaseService *m_dbService = nullptr;

    QSharedPointer<IDbModel> m_dbModel;

    QHash<QString, QPointer<IDbModel>> m_dbModelMap{};

    QHash<QString, QPointer<QAbstractTableModel>> m_abstractModelsMap{};
};

#endif // DBMODELCONTROLLER_H
