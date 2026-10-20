#ifndef SYSTEMENVIRONMENT_H
#define SYSTEMENVIRONMENT_H

#include <QObject>
#include <QDir>

class SystemEnvironment : public QObject
{
public:
    SystemEnvironment(QObject *parent);

    const QString currentMacAddress() const;
    QDir dir();
};

#endif // SYSTEMENVIRONMENT_H
