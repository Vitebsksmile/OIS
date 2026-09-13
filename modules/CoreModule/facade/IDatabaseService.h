#ifndef IDATABASESERVICE_H
#define IDATABASESERVICE_H

#include <QObject>
//#include <QSharedPointer>

class IDbModel;

class IDatabaseService : public QObject
{
    Q_OBJECT

public:
    explicit IDatabaseService(QObject *parent = nullptr) : QObject(parent) {}
    virtual ~IDatabaseService() = default;

    //  Initialize database
    virtual bool initDatabase(const QString& dbName) = 0;

    //  Model returning a method
    virtual IDbModel *itemModel() const = 0;
    virtual IDbModel *model(const QString &) const = 0;

    virtual bool logNewDefect(int boardId,
                              int typeId,
                              const QString &designator,
                              double x,
                              double y) = 0;

    //  Method that returning a list tables
    virtual const QStringList availableTables() const = 0;

signals:
    void sessionAdded();
    void operatorAdded();
    void computersAdded();
    void cameraAdded();
    void defectAdded();
};

//  Factory method
QSharedPointer<IDatabaseService> createDatabaseService(QObject *parent);

#endif // IDATABASESERVICE_H
