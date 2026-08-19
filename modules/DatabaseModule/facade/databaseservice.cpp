#include "databaseservice.h"
#include "databasemanager.h"

DatabaseService::DatabaseService(QObject *parent)
    : IDatabaseService(parent)
    , m_databaseManager(&DatabaseManager::instance())
{
    bool isBdReady = m_databaseManager->initDatabase();
    if (isBdReady) {
        qDebug() << "DatabaseService: Database successfuly initialized upon service startup.";
    } else {
        qDebug() << "DatabaseService: CRITICAL ERROR: failed to start the database";
    }
}
