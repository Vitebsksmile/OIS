#include "yolodetectionworker.h"
#include <QDebug>
#include <opencv2/imgproc.hpp>
#include <QThread>

YoloDetectionWorker::YoloDetectionWorker(const QString &modelPath, QObject *parent)
    : QObject(parent)
    , m_modelPath(modelPath)
{
    qDebug() << "YoloDetectionWorker created in thread:"
             << QThread::currentThreadId();

    // if (!m_detector->isReady()) {
    //     qWarning() << "YoloDetectionWorker: detector is not ready";

        //emit error("ONNX detector is not ready");
    // }
}

YoloDetectionWorker::~YoloDetectionWorker()
{
    //this->stopCapture();
}

// void YoloDetectionWorker::startCapture(const cv::Mat &cvFrame)
// {
//     cv::Mat localFrame = cvFrame;

//     if (m_detector && m_detector->isReady()) {

//         const QVector<Core::Detection> detections = m_detector->detect(localFrame);

//         qDebug()
//             << "YoloDetectionWorker: neural network detections:"
//             << detections.size();

//         for (const Core::Detection &detection : detections) {
//             qDebug() << "class:"         << detection.className
//                      << "confidence:"    << detection.confidence
//                      << "box:"           << detection.boundingBox;
//         }

//         emit detectionsReady(cvFrame, detections);
//     }
// }

// void YoloDetectionWorker::stopCapture()
// {
//     if (m_detector->isReady()) {
//         qDebug() << "YoloDetectionWorker::stopCapture(). m_detector =" << m_detector;
//         m_detector = nullptr;
//         qDebug() << "YoloDetectionWorker::stopCapture(). m_detector =" << m_detector;
//     }
// }

void YoloDetectionWorker::processFrame(const cv::Mat &cvFrame)
{
    //  Сообщаем о завершении, чтобы ProcessManager смог отправить следующий кадр
    if (cvFrame.empty()) {
        emit detectionFinished();
        return;
    }

    if (!m_initialized || !m_detector || !m_detector->isReady()) {
        emit errorOccurred("YOLO detector is not ready");
        emit detectionFinished();
        return;
    }

    try {
        const QVector<Core::Detection> detections = m_detector->detect(cvFrame);
        emit detectionsReady(cvFrame, detections);

    } catch (const std::exception &e) {
        emit errorOccurred(QString("YOLO inference error: %1")
                               .arg(QString::fromLocal8Bit(e.what())));
    } catch (...) {
        emit errorOccurred("YOLO inference failed: unknown error");
    }

    emit detectionFinished();
}

void YoloDetectionWorker::initialize()
{
    qDebug() << "YoloDetectionWorker: YOLO worker thread:"
             << QThread::currentThread();

    try {
        m_detector = new OnnxDefectDetector(m_modelPath);
        m_initialized = m_detector->isReady();

        if (m_initialized) {
            qDebug() << "YoloDetectionWorker: YOLO worker: ONNX model is ready";
        } else {
            emit errorOccurred("YoloDetectionWorker: YOLO worker: failed to initialize ONNX model");
        }
    } catch (const std::exception &e) {
        m_detector->deleteLater();
        m_detector = nullptr;
        m_initialized = false;

        emit errorOccurred(QString("YoloDetectionWorker: YOLO model initialization error: %1")
                               .arg(QString::fromLocal8Bit(e.what())));
    } catch (...) {
        m_detector->deleteLater();
        m_detector = nullptr;
        m_initialized = false;

        emit errorOccurred("YoloDetectionWorker: YOLO model initialization failed: unknown error");
    }
}
