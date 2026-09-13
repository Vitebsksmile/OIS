#ifndef DBMODELCONTROLLER_H
#define DBMODELCONTROLLER_H

#include <QObject>
#include <QtQml/qqmlregistration.h>
#include <QQmlEngine>
#include <QSharedPointer>
#include <QAbstractItemModel>
#include "IVisualizationService.h"
#include "IDbModel.h"
#include "IDatabaseService.h"
#include "videoProvider.h"

class DbModelController : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(QAbstractItemModel *operatorsModel READ operatorsModel NOTIFY operatorsModelChanged)
    Q_PROPERTY(QAbstractItemModel* currentModel READ currentModel NOTIFY currentModelChanged)
    QML_SINGLETON
    QML_ELEMENT
    // Запрещаем создавать объект этого типа напрямую из QML:
    QML_UNCREATABLE("Interface")

    static DbModelController *s_instance;

public:
    explicit DbModelController(IVisualizationService *visualization,
                               QObject *parent = nullptr);

    static DbModelController* create(QQmlEngine *, QJSEngine *) { return s_instance; }

    Q_INVOKABLE void registerProvider(VideoProvider *provider = nullptr);

    void setDbService(IDatabaseService *dbService);
    Q_INVOKABLE QAbstractItemModel* dbModel(const QString &tableName) const;
    QAbstractItemModel* currentModel() const;

    void setOperatorsModel(IDbModel *dbModel);
    void updateModel(IDbModel *dbModel);

    Q_INVOKABLE void logAbstractModel(QAbstractItemModel *model);
    Q_INVOKABLE void logQmlModelRoles(QAbstractItemModel *model);

    // OperatorsController(const OperatorsController& ) = delete;
    // OperatorsController& operator=(const OperatorsController&) = delete;

    //Q_INVOKABLE void save() { m_dbModel->saveChanges(); }

signals:
    void operatorsModelChanged();
    void currentModelChanged();

private:
    void populateModelsMap();

    IVisualizationService *m_visualization = nullptr;
    IDatabaseService *m_dbService = nullptr;

    QList<QPointer<VideoProvider>> m_providers;

    QSharedPointer<IDbModel> m_dbModel;

    QHash<QString, QPointer<IDbModel>> m_dbModelMap{};
    QAbstractItemModel *m_currentModel = nullptr;
};

#endif // DBMODELCONTROLLER_H
