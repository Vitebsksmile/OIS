#include "databasemanager.h"

DatabaseManager *DatabaseManager::instance()
{
    if (m_instance == nullptr) {
        m_instance = new DatabaseManager();
    }
    return m_instance;
}

bool DatabaseManager::initDatabase()
{
    //  Creat a connect to the database driver and database name
    m_db = QSqlDatabase::addDatabase("QSQLITE");

    //  Get the path to the application data folder
    //  (for example, C:/Users/Name/AppData/Roaming/YourApp)
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    //  Creat the directory if it does not already exist
    QDir dir(appDataPath);
    if (!dir.exists()) { dir.mkpath("."); }

    //  Construct the full path to the database file
    QString dbPath = dir.absoluteFilePath("/users.db");
    m_db.setDatabaseName(dbPath);

    if (!m_db.open())
    {
        qCritical()
        << "Error opening database"
        << m_db.lastError().text();
        return false;
    }

    qDebug()
        << "Database has been successfully connected to path:"
        << dbPath;
    return this->creatTables();
}

bool DatabaseManager::creatTables()
{
    QSqlQuery query;

    //  Creat the users table if it does not already exist
    //  TEXT UNIQUE prevents the creation of two users with the same login
    QString strQuery = "CREAT TABLE IF NOT EXISTS users ("
                       "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                       "username TEXT UNIQUE NOT NULL, "
                       "password TEXT NOT NULL)";

    if (!query.exec(strQuery))
    {
        qCritical()
        << "Error creating table"
        << query.lastError().text();
        return false;
    }
    return true;
}
