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
    void startCaptureUrl();
    void startCaptureUrl(const QString &url);
    void startCapture(int cameraIndex = 0);

    void stopCapture();

signals:
    void rawCVFrameReady(const cv::Mat &cvFrame);
    void rawImageFrameReady(const QImage imageFrame);
    void customFrameReady(const CVFrameBuffer &frame);

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
