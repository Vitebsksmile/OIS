#include "yolodetectionworker.h"

#include <QDebug>
#include <opencv2/imgproc.hpp>
#include <QThread>

YoloDetectionWorker::YoloDetectionWorker(const QString &modelPath, QObject *parent)
    : QObject(parent)
    , m_detector(new OnnxDefectDetector(modelPath))
{
    qDebug() << "YoloDetectionWorker created in thread:"
             << QThread::currentThread();

    if (!m_detector->isReady()) {
        qWarning() << "YoloDetectionWorker: detector is not ready";

        emit error("ONNX detector is not ready");
    }
}

void YoloDetectionWorker::processFrame(const cv::Mat &frame)
{
    if (!m_detector || !m_detector->isReady()) { return; }

    if (frame.empty()) { return; }

    const QVector<Core::Detection> detections = m_detector->detect(frame);

    qDebug() << "YoloDetectionWorker:"
             << "detections =" << detections.size();

    emit detectionsReady(QImage(), // пока обсудим ниже
                         detections);
}
