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
    void onFrameReady(const QImage img);

    void onFrameWithBoxesReady(const QImage &frame,
                               const std::vector<std::vector<int>> &rectanglePoints);

signals:
    void videoSinkChanged();

private:
    void processFrame(const QImage img);

private:
    QVideoSink* m_videoSink;

    std::vector<std::vector<int>> m_smoothedBoxes;

    QString m_frameSource;
};

#endif // VIDEOPROVIDER_H
