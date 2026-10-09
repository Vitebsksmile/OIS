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
    Q_INVOKABLE bool creatCamera(int cameraIndex);
    Q_INVOKABLE void disconnectCamera();
    Q_INVOKABLE void changesDetectionMethod(const QString &detectionMethod);
    Q_INVOKABLE void enableDetection(bool detection);

signals:
    void errorOccurred(const QString &errorTitle,
                       const QString &errorDetails = "");

private:
    VisualizationService *m_service = nullptr;
};

#endif // SESSIONCONTROLLER_H
