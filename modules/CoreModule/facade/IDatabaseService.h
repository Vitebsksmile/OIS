#ifndef IDATABASESERVICE_H
#define IDATABASESERVICE_H

#include <QObject>
#include <QAbstractTableModel>
#include "dbrecord.h"
#include "Core.h"

class IDbModel;

class IDatabaseService : public QObject
{
    Q_OBJECT

public:
    explicit IDatabaseService(QObject *parent = nullptr) : QObject(parent) {}
    virtual ~IDatabaseService() = default;

    //  Initialize database
    virtual bool initDatabase(const QString& dbName) = 0;

    //  Model returning a method
    virtual IDbModel* itemModel() const = 0;
    virtual IDbModel* model(const QString &tableName) const = 0;
    virtual QAbstractTableModel* abstractTableModel(const QString &tableName) = 0;

    virtual bool logNewDefect(int boardId,
                              int typeId,
                              const QString &designator,
                              double x,
                              double y) = 0;

    virtual const Core::AuthResult authenticate(const QString &username,
                                                const QString &password,
                                                const QString &message = "Authentication user") = 0;

    virtual const Core::DbOperationResult creatUser(const QString &username,
                                                    const QString &password,
                                                    const QString &fullName,
                                                    const QString &jobTitle,
                                                    const QString &message = "New user creat") = 0;

    virtual const Core::DbOperationResult creatCamera(const QString &address,
                                                      const QString &message = "New camera creat") = 0;

    virtual const Core::SessionContext creatSession(Core::AuthResult result) = 0;

    // virtual const Core::DbOperationResult findRecord(const QString &tableName,
    //                                                  const Core::DbRecord &record,
    //                                                  const QString &message) = 0;

    //  Method that returning a list tables
    virtual const QStringList availableTables() const = 0;

signals:
    void cameraAdded();
    void defectAdded();

    //void dbExecutionError(const QString &error);
};

//  Factory method
QSharedPointer<IDatabaseService> createDatabaseService(QObject *parent);

#endif // IDATABASESERVICE_H
