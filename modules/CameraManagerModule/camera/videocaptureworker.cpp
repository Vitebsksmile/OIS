#include "videocaptureworker.h"

#include <QDebug>

VideoCaptureWorker::VideoCaptureWorker(QObject *parent)
    : QObject(parent), m_timer(nullptr)
{
    m_timer = new QTimer(this);

    connect(m_timer, &QTimer::timeout,
            this, &VideoCaptureWorker::processFrame);
}

VideoCaptureWorker::~VideoCaptureWorker()
{
    stopCapture();
}

void VideoCaptureWorker::startCapture(int cameraIndex)
{
    stopCapture();  //  Останавливаем предыдущий захват перед стартом нового

    if (!m_cap.open(cameraIndex)) {
        qWarning()
            << "Не удалось открыть локальную камеру с индексом:"
            << cameraIndex;
        return;
    }
    m_timer->start(33); //  1000 мс / 33 мс = 30.3 FPS
    qDebug() << "Successfully connected to the local camera";
}

// void VideoCaptureWorker::startCaptureUrl(const QString &url)
// {
//     stopCapture();

//     if (!m_cap.open(url.toStdString())) {
//         qWarning()
//             << "Не удалось открыть сетевой поток DroidCam:"
//             << url;
//         return;
//     }
//     m_timer->start(33); //  1000 мс / 33 мс = 30.3 FPS
//     qDebug() << "Successfully connected to DroidCam via Wi-Fi";
// }

void VideoCaptureWorker::startCaptureUrl()
{
    stopCapture();
    QString url = "http://192.168.100.14:4747/video";

    if (!m_cap.open(url.toStdString())) {
        qWarning()
            << "Не удалось открыть сетевой поток DroidCam:"
            << url;
        return;
    }
    m_timer->start(33); //  1000 мс / 33 мс = 30.3 FPS
    qDebug() << "VideoCaptureWorker: Successfully connected to DroidCam via Wi-Fi";
}

void VideoCaptureWorker::stopCapture()
{
    if (m_timer && m_timer->isActive()) {
        m_timer->stop();
    }
    if (m_cap.isOpened()) {
        m_cap.release();
    }
}

//  SLOT QTimer::timeout -> this
void VideoCaptureWorker::processFrame()
{
    cv::Mat mat;
    if (m_cap.read(mat) && !mat.empty()) {
        emit cvFrameReady(mat);
        QImage imageFrame = matToQImage(mat);
        if (!imageFrame.isNull()) {
            emit imageFrameReady(imageFrame);
        }
    }
}

QImage VideoCaptureWorker::matToQImage(const cv::Mat &mat)
{
    if (mat.type() == CV_8UC3) {
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
