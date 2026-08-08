#include "videoProvider.h"
#include <QDebug>

VideoProvider::VideoProvider(QObject* parent)
    : QObject(parent)
    , m_videoSink(nullptr)
{
}

void VideoProvider::setVideoSink(QVideoSink* sink)
{
    // Выводим лог при любой попытке QML передать указатель
    qDebug() << "VideoProvider: setVideoSink вызван! Адрес sink:" << sink;

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

void VideoProvider::onFrameReady(const QImage img)
{
    if (!img.isNull())
    {
        this->processFrame(img);
    }
}

void VideoProvider::processFrame(const QImage img)
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
    }
}
