#ifndef IDATABASESERVICE_H
#define IDATABASESERVICE_H

#include <QObject>

class IDatabaseService : public QObject
{
    Q_OBJECT

public:
    explicit IDatabaseService(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~IDatabaseService() = default;
};

#endif // IDATABASESERVICE_H
