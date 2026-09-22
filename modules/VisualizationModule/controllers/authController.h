#ifndef AUTHCONTROLLER_H
#define AUTHCONTROLLER_H

#include <QObject>
#include <QtQml/qqmlregistration.h>
#include <QQmlEngine>
#include "visualizationservice.h"
#include "dbmodelcontroller.h"

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

    Q_INVOKABLE bool login(const QString &username, const QString &password);
    Q_INVOKABLE bool identification(const QString &username);
    Q_INVOKABLE bool autentification(const QString &username, const QString &password);

signals:
    void identificationSuccess();
    void authSuccess();
    void authFailed(const QString &error);

private:
    VisualizationService *m_visualization = nullptr;

    DbModelController *m_dbController = nullptr;
    int m_identificationColumn = -1;
    int m_identificationRow = -1;
    QString m_username;
    QString m_fullname;
};

#endif // AUTHCONTROLLER_H
