#ifndef OPENCVVIDEOITEM_H
#define OPENCVVIDEOITEM_H

#include <QQuickPaintedItem>
#include <QImage>
#include <QtQml/qqmlregistration.h>

class OpenCVVideoItem : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString streamUrl
                   READ streamUrl
                       WRITE setStreamUrl
                           NOTIFY streamUrlChanged FINAL)

public:
    explicit OpenCVVideoItem(QQuickItem *parent = nullptr);
    ~OpenCVVideoItem();

    void paint(QPainter *painter) override;

    QString streamUrl() const { return m_streamUrl; }

    void setStreamUrl(const QString &url) {
        if (m_streamUrl != url) {
            m_streamUrl = url;
            emit streamUrlChanged();
        }
    }

signals:
    void streamUrlChanged();

public slots:
    void startStream();
    void updateFrame(const QImage &frame);

private:
    QString m_streamUrl;
    QImage m_currentFrame;
    //VideoCaptureWorker *m_worker;
};

#endif // OPENCVVIDEOITEM_H
