#ifndef ICAMERAMANAGERSERVICE_H
#define ICAMERAMANAGERSERVICE_H

#include <QObject>


class ICameraManagerService : public QObject
{
    Q_OBJECT

public:
    explicit ICameraManagerService(QObject* parent = nullptr) : QObject(parent) {}

    virtual ~ICameraManagerService() = default;

public slots:

signals:

};

#endif // ICAMERAMANAGERSERVICE_H
