#include "sessionrepository.h"
#include "systemenvironment.h"
#include "dbrecord.h"
#include "sqlquerybuilder.h"

SessionRepository::SessionRepository(SystemEnvironment *sysEnvironment, SqlQueryBuilder* builder, QObject *parent)
    : QObject(parent)
    , m_sysEnvironment(sysEnvironment)
    , m_builder(builder)
{}

const Core::SessionContext SessionRepository::creatSession(Core::AuthResult authResult)
{
    QString tableName = "sessions";
    Core::SessionContext context;

    QString macAddress = m_sysEnvironment->currentMacAddress();
    int computerId = m_builder->currentComputerId(macAddress);

    Core::DbRecord record;
    QDateTime startedAt = QDateTime::currentDateTime();
    record.insert("operator_id", authResult.operatorId);
    record.insert("computer_id", computerId);
    record.insert("started_at", startedAt);

    Core::DbOperationResult operationResult = m_builder->insertRecord(tableName, record, "Creat session");
    context.active = operationResult.success;
    context.context = operationResult.error;
    context.operatorId = authResult.operatorId;
    context.username = authResult.username;
    context.fullName = authResult.fullName;
    context.sessionId = operationResult.insertedId;

    qDebug() << "SessionRepository:" << "\n"
             << "context.active =" << context.active << "\n"
             << "context.context =" << context.context << "\n"
             << "context.operatorId =" << context.operatorId << "\n"
             << "context.username =" << context.username << "\n"
             << "context.fullName =" << context.fullName << "\n"
             << "context.sessionId =" << context.sessionId;

    return context;
}

const Core::SessionContext SessionRepository::finishSession(Core::SessionContext &context)
{
    QString tableName = "sessions";
    int sessionId = context.sessionId;

    Core::DbRecord record;
    QDateTime finishedAt = QDateTime::currentDateTime();

    record.insert("finished_at", finishedAt);

    Core::DbOperationResult operationResult = m_builder->updateSession(tableName, sessionId, finishedAt, "Finished session");
    context.active = !operationResult.success;

    qDebug() << "SessionRepository:" << "\n"
             << "context.active =" << context.active << "\n"
             << "context.context =" << context.context << "\n"
             << "context.operatorId =" << context.operatorId << "\n"
             << "context.username =" << context.username << "\n"
             << "context.fullName =" << context.fullName << "\n"
             << "context.sessionId =" << context.sessionId;

    return context;
}
