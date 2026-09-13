#ifndef VIDEOCAPTUREWORKER_H
#define VIDEOCAPTUREWORKER_H

#include <QObject>
#include <QImage>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include "cvframebuffer.h"

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
    void imageFrameReady(const QImage imageFrame);
    void cvFrameReady(const cv::Mat &cvFrame);
    void frameReady(const CVFrameBuffer &frame);

private slots:
    void processFrame();

private:
    QImage matToQImage(const cv::Mat &mat);
    CVFrameBuffer matToFrame(const cv::Mat &mat);

private:
    cv::VideoCapture m_cap;
    QTimer *m_timer;
};

#endif // VIDEOCAPTUREWORKER_H
