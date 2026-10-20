#include "userrepository.h"
#include <QRandomGenerator>
#include <QCryptographicHash>
#include "sqlquerybuilder.h"

UserRepository::UserRepository(SqlQueryBuilder* builder, QObject *parent)
    : QObject(parent)
    , m_builder(builder)
{}

const Core::DbOperationResult UserRepository::createUser(const QString &username,
                                                         const QString &password,
                                                         const QString &fullName,
                                                         const QString &jobTitle,
                                                         const QString &message)
{
    if (password.isEmpty()) {
        QString userMessage = QString("Не удалось выполнить операцию: %1.\n\n").arg(message);
        Core::DbOperationResult result;
        result.error = "Password is empty!";
        return result;
    }
    // Генерация случайной соли (например, 16 символов)
    QString salt = QString::number(QRandomGenerator::global()->generate64(), 36);
    // Создание хэша
    QByteArray hashPassword = QCryptographicHash::hash((password + salt).toUtf8(),
                                                       QCryptographicHash::Sha256).toHex();

    Core::DbRecord record;
    record.insert("username", username);
    record.insert("full_name", fullName);
    record.insert("password_hash", hashPassword);
    record.insert("salt", salt);

    Core::DbOperationResult result = m_builder->insertRecord("operators", record, message);

    return result;
}
