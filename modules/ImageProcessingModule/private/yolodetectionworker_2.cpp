#include "yolodetectionworker_2.h"

#include <QDebug>
#include <QThread>

#include <opencv2/imgproc.hpp>

YoloDetectionWorker_2::YoloDetectionWorker_2(const QString &modelPath,
                                             QObject *parent)
    : QObject(parent)
    , m_modelPath(modelPath)
{
    // Не загружаем модель здесь:
    // конструктор вызывается до moveToThread().
}

void YoloDetectionWorker_2::initialize()
{
    qDebug() << "YOLO worker thread:"
             << QThread::currentThread();

    try {
        m_detector = std::make_unique<OnnxDefectDetector>(m_modelPath);

        m_initialized = m_detector->isReady();

        if (m_initialized) {
            qDebug() << "YOLO worker: ONNX model is ready";
        } else {
            emit errorOccurred("YOLO worker: failed to initialize ONNX model");
        }
    } catch (const std::exception &e) {
        m_detector.reset();
        m_initialized = false;

        emit errorOccurred(QString("YOLO model initialization error: %1")
                               .arg(QString::fromLocal8Bit(e.what())));
    } catch (...) {
        m_detector.reset();
        m_initialized = false;

        emit errorOccurred("YOLO model initialization failed: unknown error");
    }
}

void YoloDetectionWorker_2::processFrame(const cv::Mat &frame)
{
    // Всегда сообщаем о завершении, чтобы ProcessManager
    // мог разрешить отправку следующего кадра.
    if (frame.empty()) {
        emit processingFinished();
        return;
    }

    if (!m_initialized || !m_detector || !m_detector->isReady()) {
        emit errorOccurred("YOLO detector is not ready");
        emit processingFinished();
        return;
    }

    try {
        const QVector<Core::Detection> detections = m_detector->detect(frame);

        const QImage image = matToQImage(frame);

        if (image.isNull()) {
            emit errorOccurred("YOLO worker: failed to convert frame to QImage");
            emit processingFinished();
            return;
        }

        emit detectionsReady(image, detections);
    } catch (const std::exception &e) {
        emit errorOccurred(QString("YOLO inference error: %1")
                               .arg(QString::fromLocal8Bit(e.what())));
    } catch (...) {
        emit errorOccurred("YOLO inference failed: unknown error");
    }

    emit processingFinished();
}

QImage YoloDetectionWorker_2::matToQImage(const cv::Mat &mat) const
{
    if (mat.empty()) {
        return {};
    }

    if (mat.type() == CV_8UC3) {
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);

        return QImage(rgb.data,
                      rgb.cols,
                      rgb.rows,
                      static_cast<qsizetype>(rgb.step),
                      QImage::Format_RGB888).copy();
    }

    if (mat.type() == CV_8UC1) {
        return QImage(mat.data,
                      mat.cols,
                      mat.rows,
                      static_cast<qsizetype>(mat.step),
                      QImage::Format_Grayscale8).copy();
    }

    if (mat.type() == CV_8UC4) {
        return QImage(mat.data,
                      mat.cols,
                      mat.rows,
                      static_cast<qsizetype>(mat.step),
                      QImage::Format_RGBA8888).copy();
    }

    return {};
}
