#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>

class DatabaseManager
{
public:
    DatabaseManager(const DatabaseManager& ) = delete;

    static DatabaseManager& instance();

    bool initDatabase();

private:
    DatabaseManager() {}
    ~DatabaseManager() {}

    bool creatTables();
    QDir dir();
private:
    QSqlDatabase m_db;
};

#endif // DATABASEMANAGER_H
