#ifndef YOLODETECTIONWORKER_H
#define YOLODETECTIONWORKER_H

#include <QObject>
#include <QImage>
#include <QVector>
#include <opencv2/core.hpp>
#include "onnxdefectdetector.h"

class YoloDetectionWorker : public QObject
{
    Q_OBJECT

public:
    explicit YoloDetectionWorker(const QString &modelPath,
                                 QObject *parent = nullptr);

public slots:
    void processFrame(const cv::Mat &frame);

signals:
    void detectionsReady(const QImage &frame,
                         const QVector<Core::Detection> &detections);

    void error(const QString &message);

private:
    OnnxDefectDetector *m_detector = nullptr;
};

#endif