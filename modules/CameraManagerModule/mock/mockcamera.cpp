#include "mockcamera.h"

#include <QDebug>
#include <memory>

#include "IFrameObserver.h"
#include "mockframebuffer.h"


MockCamera::MockCamera(
    OIS::Core::CameraConfig config,
    QObject* parent
    )
    : QObject(parent)
    , m_config(std::move(config))
{
    m_timer = new QTimer(this);

    connect(
        m_timer,
        &QTimer::timeout,
        this,
        &MockCamera::generateFrame
        );
}


MockCamera::~MockCamera()
{
    stopGrab();

    close();
}


bool MockCamera::open()
{
    if (
        m_state !=
        OIS::Core::CameraState::Disconnected
        )
    {
        return false;
    }

    m_state = OIS::Core::CameraState::Connected;

    qDebug()
        << "MockCamera: open camera"
        << m_config.id;

    return true;
}


void MockCamera::close()
{
    if (m_state == OIS::Core::CameraState::Disconnected)
    {
        return;
    }

    qDebug()
        << "MockCamera: close camera"
        << m_config.id;

    m_state = OIS::Core::CameraState::Disconnected;
}


bool MockCamera::startGrab()
{
    if (m_state != OIS::Core::CameraState::Connected)
    {
        return false;
    }

    const int interval = m_config.fps > 0.0 ? static_cast<int>(1000.0 / m_config.fps) : 33;

    m_state = OIS::Core::CameraState::Grabbing;

    qDebug()
        << "MockCamera: start grabbing"
        << m_config.id;

    return true;
}


void MockCamera::stopGrab()
{
    if (m_timer)
    {
        m_timer->stop();
    }

    if (m_state != OIS::Core::CameraState::Grabbing)
    {
        return;
    }

    qDebug()
        << "MockCamera: stop grabbing"
        << m_config.id;

    m_state = OIS::Core::CameraState::Connected;
}


void MockCamera::setObserver(IFrameObserver* observer)
{
    m_observer = observer;
}


OIS::Core::CameraState
MockCamera::state() const
{
    return m_state;
}

void MockCamera::generateFrame()
{
    if (!m_observer)
    {
        return;
    }

    auto buffer =
        std::make_shared<MockFrameBuffer >(
            m_config.width,
            m_config.height,
            m_config.pixelFormat
            );

    OIS::Core::Frame frame;

    frame.cameraId = m_config.id;

    frame.metadata.frameNumber = m_frameNumber++;

    frame.metadata.width = m_config.width;

    frame.metadata.height = m_config.height;

    frame.metadata.pixelFormat = m_config.pixelFormat;

    frame.buffer = std::move(buffer);

    m_observer->onFrameReady(std::move(frame));

}
