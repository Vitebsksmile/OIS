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
    ~DatabaseManager() { m_instance = nullptr; }

    static DatabaseManager* instance();

    bool initDatabase();

private:
    DatabaseManager() {}

    bool creatTables();
private:
    static DatabaseManager* m_instance;
    QSqlDatabase m_db;
};

#endif // DATABASEMANAGER_H
