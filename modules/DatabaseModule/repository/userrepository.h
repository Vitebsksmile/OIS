#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include <QObject>
#include "Core.h"
#include "dbrecord.h"

//namespace core { class Core; }

class SqlQueryBuilder;

class UserRepository : public QObject
{
    Q_OBJECT
public:
    explicit UserRepository(SqlQueryBuilder* builder, QObject *parent);

    const Core::AuthResult authenticate(const QString &username,
                                        const QString &password,
                                        const QString &message = "Authentication user");

    const Core::DbOperationResult createUser(const QString &username,
                                            const QString &password,
                                            const QString &fullName,
                                            const QString &jobTitle,
                                            const QString &message = "New user creat");

private:
    //  Указатель на общий CRUD-движок
    SqlQueryBuilder* m_builder;
};

#endif // USERREPOSITORY_H
