#ifndef VIDEOSTREAMSERVICE_H
#define VIDEOSTREAMSERVICE_H

#include <QObject>
#include <QtQml/qqmlregistration.h>
#include <QQmlEngine>
#include "visualizationservice.h"
#include "videoProvider.h"

class VideoStreamService : public QObject
{
    Q_OBJECT
    QML_SINGLETON
    QML_ELEMENT
    // Запрещаем создавать объект этого типа напрямую из QML:
    QML_UNCREATABLE("Interface")

    static VideoStreamService *s_instance;

public:
    explicit VideoStreamService(VisualizationService *visualization,
                                QObject *parent = nullptr);

    static VideoStreamService* create(QQmlEngine *, QJSEngine *) { return s_instance; }

    Q_INVOKABLE void registerProvider(VideoProvider *provider = nullptr);

private:
    VisualizationService *m_visualization = nullptr;
    QList<QPointer<VideoProvider>> m_providers;
};

#endif // VIDEOSTREAMSERVICE_H
