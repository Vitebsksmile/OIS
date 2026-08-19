#ifndef DATABASESERVICE_H
#define DATABASESERVICE_H

#include "IDatabaseService.h"

class DatabaseManager;

class DatabaseService : public IDatabaseService
{
public:
    explicit DatabaseService(QObject* parent = nullptr);

private:
    DatabaseManager* m_databaseManager;
};

#endif // DATABASESERVICE_H
