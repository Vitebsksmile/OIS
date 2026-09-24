#ifndef AUTHCONTROLLER_H
#define AUTHCONTROLLER_H

#include <QObject>
#include <QtQml/qqmlregistration.h>
#include <QQmlEngine>
#include "visualizationservice.h"
#include "dbmodelcontroller.h"
#include "ListModel.h"
#include <QStandardItemModel>

class AuthController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    QML_UNCREATABLE("Controllers")

    static AuthController *s_instance;

public:
    explicit AuthController(VisualizationService *visualization,
                            QObject *parent = nullptr);

    static AuthController* create(QQmlEngine *, QJSEngine *) { return s_instance; }

    void setDbController(DbModelController *dbController);

    //identified
    Q_INVOKABLE bool existsByLogin(const QString &username);
    Q_INVOKABLE bool authenticate(const QString &username, const QString &password);
    Q_INVOKABLE bool isUsernameUnique(const QString &username);
    //Q_INVOKABLE bool registrUser(const QStringList &registerForm);
    Q_INVOKABLE bool isAuthorizated();
    Q_INVOKABLE QStandardItemModel* registrationModel();
    Q_INVOKABLE ListModel* userRegistrationModel();

signals:
    void identificationSuccess();
    void authenticationSuccess();
    void authorization();
    void authFailed(const QString &error);
    void registrationModelReady();

private slots:
    //void onRegistrationRequired();

private:
    VisualizationService *m_visualization = nullptr;

    DbModelController *m_dbController = nullptr;
    int m_identificationRow = -1;
    QString m_username;
    bool m_isAuthorizated = false;
    QString m_fullname;
    ListModel *m_registrationModel = nullptr;
};

#endif // AUTHCONTROLLER_H
