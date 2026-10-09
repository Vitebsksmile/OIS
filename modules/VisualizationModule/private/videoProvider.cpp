#include "videoProvider.h"
#include <QDebug>

VideoProvider::VideoProvider(QObject* parent)//, const QString &frameSource
    : QObject(parent)
    , m_videoSink(nullptr)
    //, m_frameSource(frameSource)
{
}

void VideoProvider::setFrameSource(const QString &frameSource)
{
    m_frameSource = frameSource;
}

void VideoProvider::setVideoSink(QVideoSink* sink)
{
    // Выводим лог при любой попытке QML передать указатель
    qDebug()
        << "VideoProvider: setVideoSink вызван! Адрес sink:"
        << sink;

    if (m_videoSink != sink)
    {
        m_videoSink = sink;
        emit videoSinkChanged();
    }

    if (m_videoSink) {
        qDebug() << "VideoProvider: Успешно привязан рабочий QVideoSink";
    } else {
        qWarning() << "VideoProvider: Внимание: QML передал NULL-указатель!";
    }
}

void VideoProvider::onRawImageFrameReady(const QImage &frame)
{
    if (!frame.isNull())
    {
        this->processFrame(frame);
    }
}

void VideoProvider::onProcessedFrameReady(const QImage &frame)
{
    if (!frame.isNull())
    {
        this->processFrame(frame);
    }
}

void VideoProvider::onFrameWithBoxesReady(const QImage &frame
                                          , const std::vector<std::vector<int>> &rectanglePoints)
{
    QImage editableFrame = frame;
    if (editableFrame.isNull()) { return; }

    QPainter painter(&editableFrame);

    QPen pen(Qt::green
             , 6
             , Qt::SolidLine
             , Qt::RoundCap
             , Qt::RoundJoin);

    painter.setPen(pen);

    const double alpha = 0.2;

    if (m_smoothedBoxes.size() != rectanglePoints.size())
    {
        m_smoothedBoxes = rectanglePoints;
    } else {
        for (size_t i = 0; i < rectanglePoints.size(); ++i)
        {
            if (rectanglePoints[i].size() == 4 && m_smoothedBoxes[i].size() == 4)
            {
                m_smoothedBoxes[i][0] = static_cast<int>(m_smoothedBoxes[i][0] * (1.0 - alpha)
                                                         + rectanglePoints[i][0] * alpha);

                m_smoothedBoxes[i][1] = static_cast<int>(m_smoothedBoxes[i][1] * (1.0 - alpha)
                                                         + rectanglePoints[i][1] * alpha);

                m_smoothedBoxes[i][2] = static_cast<int>(m_smoothedBoxes[i][2] * (1.0 - alpha)
                                                         + rectanglePoints[i][2] * alpha);

                m_smoothedBoxes[i][3] = static_cast<int>(m_smoothedBoxes[i][3] * (1.0 - alpha)
                                                         + rectanglePoints[i][3] * alpha);
            }
        }
    }

    for (const auto& box : m_smoothedBoxes)
    {
        if (box.size() == 4)
        {
            painter.drawRect(box[0], box[1], box[2], box[3]);
        }
    }
    painter.end();

    this->processFrame(editableFrame);
}

void VideoProvider::onDetectionsReady(const QImage &frame,
                                      const QVector<Core::Detection> &detections)
{
    if (frame.isNull()) { return; }

    QImage editableFrame = frame;
    QPainter painter(&editableFrame);

    QPen pen(Qt::white
             , 6
             , Qt::SolidLine
             , Qt::RoundCap
             , Qt::RoundJoin);

    painter.setPen(pen);

    for (const Core::Detection &detection : detections) {

        const QRect &box = detection.boundingBox;

        painter.drawRect(box);

        QString label = QString("%1 %2%")
                            .arg(detection.className)
                            .arg(detection.confidence * 100.0,
                                 0,
                                 'f',
                                 1);

        painter.drawText(box.topLeft() + QPoint(0, -5),
                         label);
    }

    painter.end();
    processFrame(editableFrame);
}

void VideoProvider::processFrame(const QImage &img)
{
    if (!m_videoSink)
    {
        qDebug()
            << "!!! VideoProvider: Кадр пришел, но m_videoSink равен nullptr! Пропускаем.";
        return;
    }

    //  Create frame format
    QVideoFrameFormat format(img.size(),
                             QVideoFrameFormat::pixelFormatFromImageFormat(img.format()));

    QVideoFrame frame(format);

    //  Copy QImage data to video memory/frame buffer
    if (frame.map(QVideoFrame::WriteOnly))
    {
        memcpy(frame.bits(0),
               img.bits(),
               img.sizeInBytes());
        //  Освобождаем память (размапливаем кадр) ДО отправки в QVideoSink!
        frame.unmap();

        //  Send frame directly to QML VideoOutput
        m_videoSink->setVideoFrame(frame);
    } else {
        qCritical() << "VideoProvider: NOT frame.map(QVideoFrame::WriteOnly)";
    }
}
