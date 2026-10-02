#ifndef SESSIONCONTROLLER_H
#define SESSIONCONTROLLER_H

#include <QObject>
#include <QtQml/qqmlregistration.h>
#include <QQmlEngine>
#include "visualizationservice.h"

class SessionController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    QML_UNCREATABLE("Controllers")

    static SessionController *s_instance;

public:
    explicit SessionController(VisualizationService *service,
                               QObject *parent = nullptr);

    static SessionController* create(QQmlEngine *, QJSEngine *) { return s_instance; }

    Q_INVOKABLE bool creatCamera(const QString &url);

signals:
    void errorOccurred(const QString &errorTitle,
                       const QString &errorDetails = "");

private:
    VisualizationService *m_service = nullptr;
};

#endif // SESSIONCONTROLLER_H
