#ifndef ICAMERASTRATEGY_H
#define ICAMERASTRATEGY_H

#include <QObject>


class ICameraStrategy : public QObject
{
    Q_OBJECT

public:
    explicit ICameraStrategy(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~ICameraStrategy() = default;

    virtual bool connect() = 0;
    virtual void startGrabbing() = 0;
    //virtual ImageFrame grabFrame() = 0;
    virtual void stopGrabbing() = 0;
    virtual void disconnect() = 0;
};

#endif // ICAMERASTRATEGY_H
