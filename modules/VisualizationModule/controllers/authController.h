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

    Q_INVOKABLE bool login(const QString &username,
                           const QString &password);

signals:
    void authSuccess();
    void authFailed(const QString &error);

private:
    VisualizationService *m_visualization = nullptr;

    DbModelController *m_dbController = nullptr;
};

#endif // AUTHCONTROLLER_H
