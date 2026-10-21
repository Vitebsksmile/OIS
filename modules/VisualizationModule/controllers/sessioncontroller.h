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

    Q_PROPERTY(QString operatorName
            READ operatorName
                NOTIFY statusChanged)

    Q_PROPERTY(int countPass
                   READ countPass
                       NOTIFY countPassChanged)

    Q_PROPERTY(int countFail
                   READ countFail
                       NOTIFY countFailChanged)

    static SessionController *s_instance;

public:
    explicit SessionController(VisualizationService *service,
                               QObject *parent = nullptr);

    static SessionController* create(QQmlEngine *, QJSEngine *) { return s_instance; }

    void startSession(const Core::AuthResult &result);
    void finishCurrentSession();

    Q_INVOKABLE bool creatCamera(const QString &url);
    Q_INVOKABLE bool creatCamera(int cameraIndex);
    Q_INVOKABLE void disconnectCamera();
    Q_INVOKABLE void toggleTrigger();
    Q_INVOKABLE void setCountFail();
    Q_INVOKABLE void setCountPass();

    QString operatorName() const;
    int countPass() const;
    int countFail() const;

public slots:
    void onCropPCBReady(const QImage &cropPCB);
    void onDefectiveBoard();

signals:
    void triggered();
    void errorOccurred(const QString &errorTitle,
                       const QString &errorDetails = "");

    void statusChanged();
    void countPassChanged();
    void countFailChanged();

    void defectiveBoard();

private:
    VisualizationService *m_service = nullptr;

    Core::SessionContext m_currentSession;

    int m_countPass = 0;
    int m_countFail = 0;
};

#endif // SESSIONCONTROLLER_H
