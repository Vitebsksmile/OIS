#include "cameramanagerservice.h"

#include <QDebug>

#include "ICamera.h"
#include "ICameraFactory.h"
#include "ICameraFactoryRegistry.h"


CameraManagerService::CameraManagerService(
    std::unique_ptr<ICameraFactoryRegistry> factoryRegistry,
    QObject* parent)
    : ICameraManagerService(parent)
    , m_factoryRegistry(std::move(factoryRegistry)
                        )
{
}

CameraManagerService::~CameraManagerService()
{
    shutdown();
}


bool CameraManagerService::initialize()
{
    if (m_initialized)
    {
        return true;
    }

    if (!m_factoryRegistry)
    {
        qWarning()
        << "CameraManagerService:"
        << "Factory registry is null";

        return false;
    }

    m_initialized = true;

    qDebug()
        << "CameraManagerService initialized";

    return true;
}


void CameraManagerService::shutdown()
{
    if (!m_initialized)
    {
        return;
    }

    for (auto& [id, camera] : m_cameras)
    {
        if (!camera)
        {
            continue;
        }

        camera->stopGrab();

        camera->close();
    }

    m_cameras.clear();

    m_initialized = false;

    qDebug()
        << "CameraManagerService shutdown";
}


OIS::Core::CameraId
CameraManagerService::addCamera(
    const OIS::Core::CameraConfig& config
    )
{
    if (!m_initialized)
    {
        qWarning()
        << "CameraManagerService:"
        << "Service is not initialized";

        return 0;
    }

    if (config.id == 0)
    {
        qWarning()
        << "CameraManagerService:"
        << "Invalid camera ID";

        return 0;
    }

    if (
        m_cameras.find(config.id)
        != m_cameras.end()
        )
    {
        qWarning()
        << "CameraManagerService:"
        << "Camera already exists"
        << config.id;

        return 0;
    }

    ICameraFactory* factory =
        m_factoryRegistry->factoryFor(
            config.technology
            );

    if (!factory)
    {
        qWarning()
        << "CameraManagerService:"
        << "Factory not found";

        return 0;
    }

    std::unique_ptr<ICamera> camera =
        factory->createCamera(config);

    if (!camera)
    {
        qWarning()
        << "CameraManagerService:"
        << "Failed to create camera";

        return 0;
    }

    camera->setObserver(this);

    const auto id = config.id;

    m_cameras.emplace(
        id,
        std::move(camera)
        );

    qDebug()
        << "Camera added:"
        << id;

    return id;
}


bool CameraManagerService::removeCamera(
    OIS::Core::CameraId id
    )
{
    const auto iterator =
        m_cameras.find(id);

    if (
        iterator ==
        m_cameras.end()
        )
    {
        return false;
    }

    ICamera* camera =
        iterator->second.get();

    if (camera)
    {
        camera->stopGrab();

        camera->close();
    }

    m_cameras.erase(iterator);

    qDebug()
        << "Camera removed:"
        << id;

    return true;
}


bool CameraManagerService::startCamera(
    OIS::Core::CameraId id
    )
{
    const auto iterator =
        m_cameras.find(id);

    if (
        iterator ==
        m_cameras.end()
        )
    {
        return false;
    }

    ICamera* camera =
        iterator->second.get();

    if (!camera)
    {
        return false;
    }

    if (
        camera->state()
        ==
        OIS::Core::CameraState::Disconnected
        )
    {
        if (!camera->open())
        {
            return false;
        }
    }

    const bool started =
        camera->startGrab();

    if (started)
    {
        emit cameraStateChanged(
            id,
            camera->state()
            );
    }

    return started;
}


bool CameraManagerService::stopCamera(
    OIS::Core::CameraId id
    )
{
    const auto iterator =
        m_cameras.find(id);

    if (
        iterator ==
        m_cameras.end()
        )
    {
        return false;
    }

    ICamera* camera =
        iterator->second.get();

    if (!camera)
    {
        return false;
    }

    camera->stopGrab();

    emit cameraStateChanged(
        id,
        camera->state()
        );

    return true;
}


void CameraManagerService::onFrameReady(
    OIS::Core::Frame frame
    )
{
    if (!frame.isValid())
    {
        return;
    }

    emit frameReady(
        std::move(frame)
        );
}