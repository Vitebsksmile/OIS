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

    //bool logNewComputer();

    const Core::AuthResult authenticate(const QString &username,
                                        const QString &password,
                                        const QString &message = "Authentication user") override;

    const Core::DbOperationResult creatUser(const QString &username,
                                            const QString &password,
                                            const QString &fullName,
                                            const QString &jobTitle,
                                            const QString &message) override;

    const Core::DbOperationResult creatCamera(const QString &address,
                                              const QString &message) override;

    const Core::SessionContext creatSession(Core::AuthResult result) override;

    // const Core::DbOperationResult findRecord(const QString &tableName,
    //                                          const Core::DbRecord &record,
    //                                          const QString &message = "Find record") override;

    const QStringList availableTables() const override;

signals:


private:
    //  Initialize the database and load it into the model
    bool initDatabase(const QString &dbName) override;

    bool creatTables();
    bool insertDefaultDataIfNeeded();
    const Core::DbOperationResult insertRecord(const QString &tableName,
                                               const Core::DbRecord &record,
                                               const QString &message = "Insert record");
    bool creatModel(const QString &nameTable);

    void autoPopulateRelations(QSqlRelationalTableModel *model, const QString &displayField = "name");
    bool populateModelsMap();

    const QList<QList<QString>> findForeignTable(const QString tableName);

    int findId(const QString &tableName,
               const QString &field);

    int currentComputerId(const QString &macAddress);

    QString handleDatabaseError(const QSqlError &error,
                                const QString &contextAction);

    const QString currentMacAddress() const;
    QDir dir();

    QHash<QString, DbModel*> m_modelsMap{};
    DbModel *m_itemModel;

    QHash<QString, DbRelationalTableModel*> m_relationalModelsMap{};
};

#endif // DATABASESERVICE_H
