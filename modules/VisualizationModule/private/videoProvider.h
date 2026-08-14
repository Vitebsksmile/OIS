#ifndef VIDEOPROVIDER_H
#define VIDEOPROVIDER_H

#include <QObject>
#include <QVideoSink>
#include <QVideoFrame>
#include <QImage>
#include <QPainter>
#include <QPen>
#include <QtQml/qqmlregistration.h>


class VideoProvider : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    //  Связываем C++ и Qml: св-во, к-рое Qml будет слушать
    Q_PROPERTY(QVideoSink* videoSink
                   READ videoSink
                       WRITE setVideoSink
                           NOTIFY videoSinkChanged)

public:
    explicit VideoProvider(QObject *parent = nullptr);

    QVideoSink* videoSink() const { return m_videoSink; }

    void setVideoSink(QVideoSink* sink);

public slots:
    void onFrameReady(const QImage img);

    void onFrameWithBoxesReady(const QImage &frame
                               , const std::vector<std::vector<int>> &rectanglePoints);

signals:
    void videoSinkChanged();

private:
    void processFrame(const QImage img);

private:
    QVideoSink* m_videoSink;

    std::vector<std::vector<int>> m_smoothedBoxes;
};

#endif // VIDEOPROVIDER_H
