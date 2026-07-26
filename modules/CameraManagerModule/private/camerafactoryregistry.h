#ifndef CAMERAFACTORYREGISTRY_H
#define CAMERAFACTORYREGISTRY_H

#include <memory.h>
#include <QHash>

#include "CameraTypes.h"
#include "ICameraFactoryRegistry.h"


class ICameraFactory;

class CameraFactoryRegistry final : public ICameraFactoryRegistry
{
public:
    CameraFactoryRegistry() = default;

    ~CameraFactoryRegistry() override = default;

    void registerFactory(
        OIS::Core::CameraTechnology technology,
        std::shared_ptr<ICameraFactory> factory
        ) override;

    ICameraFactory* factoryFor(
        OIS::Core::CameraTechnology technology
        ) const override;

private:

    struct TechnologyHash
    {
        std::size_t operator()(OIS::Core::CameraTechnology technology) const noexcept
        {
            return static_cast<std::size_t>(technology);
        }
    };

    QHash<
        int,
        std::shared_ptr<ICameraFactory>
        > m_factories;
};

#endif // CAMERAFACTORYREGISTRY_H
