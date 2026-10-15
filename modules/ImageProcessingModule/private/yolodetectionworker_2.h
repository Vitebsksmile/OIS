#ifndef YOLODETECTIONWORKER_2_H
#define YOLODETECTIONWORKER_2_H

#include <QObject>
#include <QImage>
#include <QString>
#include <QVector>

#include <memory>

#include <opencv2/core.hpp>

//#include "Detection.h"
#include "onnxdefectdetector.h"

class YoloDetectionWorker_2 : public QObject
{
    Q_OBJECT

public:
    explicit YoloDetectionWorker_2(const QString &modelPath,
                                   QObject *parent = nullptr);

public slots:
    void initialize();
    void processFrame(const cv::Mat &frame);

signals:
    void detectionsReady(const QImage &frame,
                         const QVector<Core::Detection> &detections);

    void processingFinished();

    void errorOccurred(const QString &message);

private:
    QImage matToQImage(const cv::Mat &mat) const;

private:
    QString m_modelPath;
    std::unique_ptr<OnnxDefectDetector> m_detector;
    bool m_initialized = false;
};

#endif // YOLODETECTIONWORKER_2_H
