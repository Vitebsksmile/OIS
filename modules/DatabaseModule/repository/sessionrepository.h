#ifndef SESSIONREPOSITORY_H
#define SESSIONREPOSITORY_H

#include <QObject>
#include "Core.h"

class SystemEnvironment;
class SqlQueryBuilder;

class SessionRepository : public QObject
{
    Q_OBJECT
public:
    SessionRepository(SystemEnvironment *sysEnvironment, SqlQueryBuilder *builder, QObject *parent);

    const Core::SessionContext creatSession(Core::AuthResult authResult);
    const Core::SessionContext finishSession(Core::SessionContext &context);

private:
    SystemEnvironment *m_sysEnvironment;
    //  Указатель на общий CRUD-движок
    SqlQueryBuilder *m_builder;
};

#endif // SESSIONREPOSITORY_H
