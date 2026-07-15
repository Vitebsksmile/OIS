#ifndef ICAMERADRIVER_H
#define ICAMERADRIVER_H

#include <QObject>

class ICameraDriver : public QObject
{
    Q_OBJECT

public:
    explicit ICameraDriver(QObject* parent = nullptr) : QObject(parent) {}

    virtual ~ICameraDriver() = default;

    virtual bool connect() = 0;
    virtual void disconnect() = 0;
    //virtual Frame grabFrame() = 0;
    //virtual void setParameter() = 0;
    //virtual Parameter parameter() = 0;

};

#endif // ICAMERADRIVER_H
