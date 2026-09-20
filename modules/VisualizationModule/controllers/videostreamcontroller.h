#ifndef VIDEOSTREAMCONTROLLER_H
#define VIDEOSTREAMCONTROLLER_H

#include <QObject>
#include <QtQml/qqmlregistration.h>
#include <QQmlEngine>
#include "visualizationservice.h"
#include "videoProvider.h"

class VideoStreamController : public QObject
{
    Q_OBJECT
    QML_SINGLETON
    QML_ELEMENT
    // Запрещаем создавать объект этого типа напрямую из QML:
    QML_UNCREATABLE("Interface")

    static VideoStreamController *s_instance;

public:
    explicit VideoStreamController(VisualizationService *visualization,
                                QObject *parent = nullptr);

    static VideoStreamController* create(QQmlEngine *, QJSEngine *) { return s_instance; }

    Q_INVOKABLE void registerProvider(VideoProvider *provider = nullptr);

private:
    VisualizationService *m_visualization = nullptr;
    QList<QPointer<VideoProvider>> m_providers;
};

#endif // VIDEOSTREAMCONTROLLER_H
