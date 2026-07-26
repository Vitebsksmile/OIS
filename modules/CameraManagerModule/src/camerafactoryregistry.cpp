#include "camerafactoryregistry.h"

void CameraFactoryRegistry::registerFactory(
    OIS::Core::CameraTechnology technology,
    std::shared_ptr<ICameraFactory> factory)
{
    if (!factory)
    {
        return;
    }

    const int key =
        static_cast<int>(technology);

    m_factories.insert(
        key,
        std::move(factory)
        );
}

ICameraFactory* CameraFactoryRegistry::factoryFor(
    OIS::Core::CameraTechnology technology) const
{
    const int key =
        static_cast<int>(technology);

    const auto iterator =
        m_factories.constFind(key);

    if (iterator == m_factories.constEnd())
    {
        return nullptr;
    }

    return iterator.value().get();
}