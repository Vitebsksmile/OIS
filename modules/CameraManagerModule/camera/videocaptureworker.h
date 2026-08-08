#ifndef VIDEOCAPTUREWORKER_H
#define VIDEOCAPTUREWORKER_H

#include <QObject>
#include <QImage>
#include <QTimer>
#include <opencv2/opencv.hpp>

class VideoCaptureWorker : public QObject
{
    Q_OBJECT

public:
    explicit VideoCaptureWorker(QObject *parent = nullptr);
    ~VideoCaptureWorker();

public slots:
    void startCapture(int cameraIndex = 0);
    //void startCaptureUrl(const QString &url);
    void startCaptureUrl();

    void stopCapture();

signals:
    void frameReady(const QImage img);

private slots:
    void processFrame();

private:
    cv::VideoCapture m_cap;
    QTimer *m_timer;
    QImage matToQImage(const cv::Mat &mat);
};

#endif // VIDEOCAPTUREWORKER_H
