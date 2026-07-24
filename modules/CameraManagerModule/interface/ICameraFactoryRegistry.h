#ifndef ICAMERAFACTORYREGISTRY_H
#define ICAMERAFACTORYREGISTRY_H

#include <memory>

#include "CameraTypes.h"


class ICameraFactory;

class ICameraFactoryRegistry
{
public:
    virtual ~ICameraFactoryRegistry() = default;

    virtual void registerFactory(
        OIS::Core::CameraTechnology technology,
        std::shared_ptr<ICameraFactory> factory
    ) = 0;

    virtual ICameraFactory* factoryFor(
        OIS::Core::CameraTechnology technology
    ) const = 0;
};

#endif // ICAMERAFACTORYREGISTRY_H
