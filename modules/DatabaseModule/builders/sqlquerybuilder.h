#ifndef SQLQUERYBUILDER_H
#define SQLQUERYBUILDER_H

#include <QObject>
#include <QSqlError>
#include "dbrecord.h"

class SqlQueryBuilder : public QObject
{
    Q_OBJECT
public:
    SqlQueryBuilder(QObject *parent);

    const Core::DbOperationResult insertRecord(const QString &tableName,
                                               const Core::DbRecord &record,
                                               const QString &message = "Insert record");

    const Core::DbOperationResult updateSession(const QString &tableName,
                                               int sessionId,
                                               const QDateTime &finishedAt,
                                               const QString &message = "Update record");

    const Core::DbOperationResult findRecord(const QString &tableName,
                                             const Core::DbRecord &record,
                                             const QString &message = "Find record");

    int currentComputerId(const QString &macAddress);

private:
    QString handleDatabaseError(const QSqlError &error,
                                const QString &contextAction);
};

#endif // SQLQUERYBUILDER_H
