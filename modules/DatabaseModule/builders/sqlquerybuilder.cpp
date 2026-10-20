#include "sqlquerybuilder.h"
#include <QSqlDatabase>
#include <QSqlQuery>

SqlQueryBuilder::SqlQueryBuilder(QObject *parent)
    : QObject(parent)
{}

const Core::DbOperationResult SqlQueryBuilder::insertRecord(const QString &tableName,
                                                            const Core::DbRecord &record,
                                                            const QString &message)
{
    Core::DbOperationResult result;

    if (tableName.isEmpty() || record.isEmpty()) {
        result.error = "Invalid database operation";
        result.success = false;
        return result;
    }

    //  проверить существование такой таблицы
    //  проверить FK, если требуется

    //  Реализовать метод keys() в Core::DbRecord!!!!!!!
    //  чтобы избавиться от следующей строки: QVariantMap << QVariantMap
    const QVariantMap &values = record.values();
    QStringList fields = values.keys();
    QStringList placeholders;

    for (const QString &field : fields) { placeholders << ":" + field; }

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);

    const QString sql = QString("INSERT INTO %1 (%2) VALUES (%3)")
                            .arg(tableName,
                                 fields.join(", "),
                                 placeholders.join(", "));

    if (!query.prepare(sql)) {
        QString errorDetails = this->handleDatabaseError(query.lastError(), message);
        result.error = errorDetails;
        qWarning()
            << "DatabaseService: prepare INSERT failed:"
            << result.error;
        result.success = false;
        //emit dbExecutionError(error);

        return result;
    }

    for (const QString &field : fields) {
        query.bindValue(":" + field, values.value(field));
    }

    if (!query.exec()) {
        QString errorDetails = this->handleDatabaseError(query.lastError(), message);
        result.error = query.lastError().text();
        qWarning()
            << "DatabaseService: INSERT failed:"
            << result.error;
        result.success = false;
        //emit dbExecutionError(error);

        return result;
    }

    //  emit apdateModel(tableName);
    result.success = true;
    result.insertedId = query.lastInsertId().toInt();
    return result;
}

const Core::DbOperationResult SqlQueryBuilder::findRecord(const QString &tableName,
                                                          const Core::DbRecord &record,
                                                          const QString &message)
{
    Core::DbOperationResult result;

    if (tableName.isEmpty() || record.isEmpty()) {
        result.error = "Invalid database operation";
        result.success = false;
        return result;
    }

    //  Реализовать метод keys() в Core::DbRecord!!!!!!!
    //  чтобы избавиться от следующей строки: QVariantMap << QVariantMap
    const QVariantMap &values = record.values();
    QStringList fields = values.keys();
    QStringList placeholders;

    for (const QString &field : fields) { placeholders << ":" + field; }

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);

    const QString sql = QString("SELECT * FROM %1 WHERE %2 LIMIT 1")
                            .arg(placeholders.join(" AND "),
                                 tableName);

    if (!query.prepare(sql)) {
        QString errorDetails = this->handleDatabaseError(query.lastError(), message);
        result.error = errorDetails;
        qWarning()
            << "DatabaseService: prepare SELECT failed:"
            << result.error;
        result.success = false;

        return result;
    }

    for (const QString &field : fields) {
        query.bindValue(":" + field, values.value(field));
    }

    if (!query.exec()) {
        QString errorDetails = this->handleDatabaseError(query.lastError(), message);
        result.error = query.lastError().text();
        qWarning()
            << "DatabaseService: SELECT failed:"
            << result.error;
        result.success = false;

        return result;
    }

    result.success = true;
    return result;
}

int SqlQueryBuilder::currentComputerId(const QString &macAddress)
{
    int computerId = -1;

    QSqlDatabase db;
    QSqlQuery query(db);

    QString sql = "SELECT computer_id FROM computers WHERE mac_address = :mac_address LIMIT 1";

    if (!query.prepare(sql)) {
        qCritical()
        << "DatabaseService: prepare SELECT failed:"
        << query.lastError().text();
    }

    query.bindValue(":mac_address", macAddress);

    if (!query.exec()) {
        qCritical()
        << "DatabaseService: prepare SELECT failed:"
        << query.lastError().text();
        return computerId;
    }

    if (!query.next()) {
        qCritical()
        << "DatabaseService: SELECT failed:"
        << query.lastError().text();
        return computerId;
    }

    computerId = query.value(0).toInt();
    return computerId;
}

QString SqlQueryBuilder::handleDatabaseError(const QSqlError &error,
                                             const QString &contextAction)
{
    // 1. Логируем подробную информацию для разработчика
    qCritical() << "DB Error during:" << contextAction
                << "| Type:" << error.type()
                << "| Driver Text:" << error.driverText()
                << "| Database Text:" << error.databaseText();

    // 2. Формируем понятный текст для пользователя
    QString userMessage = QString("Не удалось выполнить операцию: %1.\n\n").arg(contextAction);

    // Анализируем текст ошибки (для SQLite это самый надежный способ)
    QString dbText = error.databaseText().toLower();

    if (dbText.contains("locked") || dbText.contains("busy")) {
        userMessage += "База данных временно заблокирована. Закройте другие программы, использующие этот файл, и повторите попытку.";
    } else if (dbText.contains("unique constraint failed")) {
        userMessage += "Такая запись уже существует (нарушена уникальность данных).";
    } else if (dbText.contains("disk full") || dbText.contains("ioerr")) {
        userMessage += "На устройстве недостаточно свободного места или произошел сбой диска.";
    } else if (error.type() == QSqlError::ConnectionError) {
        userMessage += "Отсутствует подключение к файлу базы данных. Проверьте путь к файлу.";
    } else {
        userMessage += "Произошла внутренняя ошибка системы. Код ошибки передан в службу поддержки.";
    }

    return userMessage;
}
