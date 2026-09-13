#ifndef AUTHCONTROLLER_H
#define AUTHCONTROLLER_H

#include <QObject>
#include <QtQml/qqmlregistration.h>
#include <QQmlEngine>
#include "IDbModel.h"


class AuthService : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    static AuthService *s_instance;

public:
    explicit AuthService(QObject *parent = nullptr)
        : QObject(parent) {}

    static AuthService* create(QQmlEngine *, QJSEngine *) { return s_instance; }

    void setDbUserModel(IDbModel *dbModel) { m_dbUserModel = dbModel; }

    Q_INVOKABLE bool login(const QString &username,
                           const QString &password)
    {
        if (username == "admin" && password == "1234") {
            emit authSuccess();
            return true;
        }
        emit authFailed("Invalid username or password!");
        return false;
    }

signals:
    void authSuccess();
    void authFailed(const QString &error);

private:
    IDbModel *m_dbUserModel = nullptr;
};

#endif // AUTHCONTROLLER_H
