#ifndef DATABASESERVICE_H
#define DATABASESERVICE_H

#include <QSqlDatabase>
#include <QDir>
#include "IDatabaseService.h"
#include "dbrelationaltablemodel.h"

class IDetectionWriteRepository;
class IDetectionModelFactory;

class SystemEnvironment;
class SqlQueryBuilder;
class UserRepository;
class SessionRepository;

class DatabaseService : public IDatabaseService
{
    Q_OBJECT

public:
    explicit DatabaseService(QObject *parent = nullptr);
    ~DatabaseService() override;

    QAbstractTableModel* abstractTableModel(const QString &tableName) override;

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

    const Core::SessionContext creatSession(Core::AuthResult authResult) override;

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

    void autoPopulateRelations(QSqlRelationalTableModel *model, const QString &displayField = "name");


    int currentComputerId(const QString &macAddress);

    // QString handleDatabaseError(const QSqlError &error,
    //                             const QString &contextAction);

    const QString currentMacAddress() const;
    QDir dir();

    QHash<QString, DbRelationalTableModel*> m_relationalModelsMap{};

    QSharedPointer<IDetectionWriteRepository> m_writeRepo;
    QSharedPointer<IDetectionModelFactory> m_modelFactory;

    SystemEnvironment *m_sysEnvironment;
    SqlQueryBuilder *m_queryBuilder;
    UserRepository *m_userRepo;
    SessionRepository *m_sessionRepo;
};

#endif // DATABASESERVICE_H
