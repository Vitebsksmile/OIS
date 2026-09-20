#ifndef DATABASESERVICE_H
#define DATABASESERVICE_H

#include <QSqlDatabase>
#include <QDir>
#include "IDatabaseService.h"
#include "dbmodel.h"
#include "dbrelationaltablemodel.h"


class DatabaseService : public IDatabaseService
{
    Q_OBJECT

public:
    explicit DatabaseService(QObject *parent = nullptr);
    ~DatabaseService() override;

    IDbModel* itemModel() const override;
    IDbModel* model(const QString &tableName) const override;
    QAbstractTableModel* abstractTableModel(const QString &tableName) override;

    bool logNewDefect(int boardId,
                      int typeId,
                      const QString &designator,
                      double x,
                      double y) override;  //  new

    const QStringList availableTables() const override;

signals:

private:
    //  Initialize the database and load it into the model
    bool initDatabase(const QString &dbName) override;

    bool creatTables();
    bool insertDefaultDataIfNeeded();
    bool creatModel(const QString &nameTable);
    bool populateModelsMap();

    QDir dir();

    bool logNewComputer();

    QSqlDatabase m_db;
    QHash<QString, DbModel*> m_modelsMap{};
    DbModel *m_itemModel;

    QHash<QString, DbRelationalTableModel*> m_relationalModelsMap;
};

#endif // DATABASESERVICE_H
