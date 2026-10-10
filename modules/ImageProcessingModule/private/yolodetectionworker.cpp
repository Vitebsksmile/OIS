#include "yolodetectionworker.h"
#include <QDebug>
#include <opencv2/imgproc.hpp>
#include <QThread>

YoloDetectionWorker::YoloDetectionWorker(const QString &modelPath, QObject *parent)
    : QObject(parent)
    , m_detector(new OnnxDefectDetector(modelPath))
{
    qDebug() << "YoloDetectionWorker created in thread:"
             << QThread::currentThreadId();

    if (!m_detector->isReady()) {
        qWarning() << "YoloDetectionWorker: detector is not ready";

        //emit error("ONNX detector is not ready");
    }
}

YoloDetectionWorker::~YoloDetectionWorker()
{
    this->stopCapture();
}

void YoloDetectionWorker::startCapture(const cv::Mat &cvFrame)
{
    cv::Mat localFrame = cvFrame;

    if (m_detector && m_detector->isReady()) {

        const QVector<Core::Detection> detections = m_detector->detect(localFrame);

        // qDebug()
        //     << "YoloDetectionWorker: neural network detections:"
        //     << detections.size();

        // for (const Core::Detection &detection : detections) {
        //     qDebug() << "class:"         << detection.className
        //              << "confidence:"    << detection.confidence
        //              << "box:"           << detection.boundingBox;
        // }

        emit detectionsReady(this->matToQImage(cvFrame), detections);
    }
}

void YoloDetectionWorker::stopCapture()
{
    if (m_detector->isReady()) {
        qDebug() << "YoloDetectionWorker::stopCapture(). m_detector =" << m_detector;
        m_detector = nullptr;
        qDebug() << "YoloDetectionWorker::stopCapture(). m_detector =" << m_detector;
    }
}

QImage YoloDetectionWorker::matToQImage(const cv::Mat &mat)
{
    if (mat.type() == CV_8UC1) {

        return QImage(mat.data,
                      mat.cols,
                      mat.rows,
                      mat.step,
                      QImage::Format_Grayscale8).copy();

    } else if (mat.type() == CV_8UC3) {
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGBA);

        //  Создаем QImage и принудительно копируем данные в кучу Qt,
        //  чтобы безопасно передать изображение через потоки.
        return QImage(rgb.data, //  сырой указатель на первый байт в памяти, где лежит матрица пикселей изображения
                      rgb.cols, //  количество столбцов матрицы
                      rgb.rows, //  количество строк матрицы
                      rgb.step, // шаг строки (stride) - полное количество байт в одной строке матрицы, ключая техническое выравнивание памяти
                      QImage::Format_RGBA8888).copy();    //  формат цвета
    }
    return QImage();
}
