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

    Q_PROPERTY(
        QString operatorName
            READ operatorName
                NOTIFY statusChanged
        )

    static AuthController *s_instance;

public:
    explicit AuthController(VisualizationService *visualization,
                            QObject *parent = nullptr);

    static AuthController* create(QQmlEngine *, QJSEngine *) { return s_instance; }

    void setDbController(DbModelController *dbController);

    Q_INVOKABLE bool existsByLogin(const QString &username);

    Q_INVOKABLE bool login(const QString &username,
               const QString &password);



    Q_INVOKABLE bool isUsernameUnique(const QString &username);

    Q_INVOKABLE bool registerUser(const QString &username,
                                  const QString &fullname,
                                  const QString &password);

    QString operatorName() const;

signals:
    void identificationSuccess();
    void authenticationSuccess();
    void authorization();
    void authFailed(const QString &errorTitle, const QString &errorDetails = "");
    //void registrationModelReady();
    void statusChanged();

private slots:
    //void onRegistrationRequired();

private:
    bool creatSession();
    VisualizationService *m_visualization = nullptr;

    DbModelController *m_dbController = nullptr;
    int m_identificationRow = -1;
    QString m_username;
    Core::AuthResult m_authResult;
    Core::SessionContext m_sessionContext;
    QString m_fullname;
    ListModel *m_registrationModel = nullptr;
};

#endif // AUTHCONTROLLER_H