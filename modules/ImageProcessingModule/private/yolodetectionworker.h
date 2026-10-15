#ifndef YOLODETECTIONWORKER_H
#define YOLODETECTIONWORKER_H

#include <QObject>
//#include <QImage>
//#include <QVector>
#include <opencv2/core.hpp>
#include "onnxdefectdetector.h"

class YoloDetectionWorker : public QObject
{
    Q_OBJECT

public:
    explicit YoloDetectionWorker(const QString &modelPath,
                                 QObject *parent = nullptr);

    ~YoloDetectionWorker();

    //void startCapture(const cv::Mat &cvFrame);
    //void stopCapture();

    void processFrame(const cv::Mat &cvFrame);

public slots:
    void initialize();

signals:
    void detectionsReady(const cv::Mat &mat,
                         const QVector<Core::Detection> &detections);

    void detectionFinished();

    void errorOccurred(const QString &message);

private:
    QString m_modelPath;
    OnnxDefectDetector *m_detector = nullptr;
    bool m_initialized = false;
};

#endif