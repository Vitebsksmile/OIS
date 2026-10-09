#ifndef VIDEOPROVIDER_H
#define VIDEOPROVIDER_H

#include <QObject>
#include <QVideoSink>
#include <QVideoFrame>
#include <QImage>
#include <QPainter>
#include <QPen>
#include <QtQml/qqmlregistration.h>
#include "Core.h"


class VideoProvider : public QObject
{
    Q_OBJECT
    //  Связываем C++ и Qml: св-во, к-рое Qml будет слушать
    Q_PROPERTY(QVideoSink* videoSink
                   READ videoSink
                       WRITE setVideoSink
                           NOTIFY videoSinkChanged)
    QML_ELEMENT

public:
    explicit VideoProvider(QObject *parent = nullptr);//, const QString &frameSource = "camera"

    QVideoSink* videoSink() const { return m_videoSink; }
    const QString& frameSource() const { return m_frameSource; }

    Q_INVOKABLE void setFrameSource(const QString &frameSource);
    void setVideoSink(QVideoSink* sink);

public slots:
    void onRawImageFrameReady(const QImage &frame);

    void onProcessedFrameReady(const QImage &frame);

    void onFrameWithBoxesReady(const QImage &frame,
                               const std::vector<std::vector<int>> &rectanglePoints);

    void onDetectionsReady(const QImage &frame,
                           const QVector<Core::Detection> &detections);

signals:
    void videoSinkChanged();

private:
    void processFrame(const QImage &img);

private:
    QVideoSink* m_videoSink;

    std::vector<std::vector<int>> m_smoothedBoxes;
    //std::vector<Core::Detection> m_smoothedDetections;

    QString m_frameSource;
};

#endif // VIDEOPROVIDER_H
