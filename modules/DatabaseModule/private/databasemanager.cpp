#include "databasemanager.h"

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::initDatabase()
{
    //  Creat a connect to the database driver and database name
    m_db = QSqlDatabase::addDatabase("QSQLITE");

    //  Construct the full path to the database file
    QString dbPath = dir().absoluteFilePath("users.db");
    m_db.setDatabaseName(dbPath);

    if (!m_db.open())
    {
        qCritical()
            << "DatabaseManager: Error opening database"
            << m_db.lastError().text();
        return false;
    }

    qDebug()
        << "DatabaseManager: Database has been successfully connected to path:"
        << dbPath;
    return this->creatTables();
}

bool DatabaseManager::creatTables()
{
    QSqlQuery query;

    //  Creat the users table if it does not already exist
    //  TEXT UNIQUE prevents the creation of two users with the same login
    QString strQuery = "CREATE TABLE IF NOT EXISTS users ("
                       "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                       "username TEXT UNIQUE NOT NULL, "
                       "password TEXT NOT NULL)";

    if (!query.exec(strQuery))
    {
        qCritical()
        << "DatabaseManager: Error creating table"
        << query.lastError().text();
        return false;
    }
    return true;
}

QDir DatabaseManager::dir()
{
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    return dir;
}
