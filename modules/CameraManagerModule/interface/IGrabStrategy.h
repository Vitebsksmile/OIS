#ifndef IGRABSTRATEGY_H
#define IGRABSTRATEGY_H

#include <functional>

#include "Frame.h"


using FrameCallback = std::function<void(OIS::Core::Frame)>;


class ICameraStrategy
{
public:
    virtual ~ICameraStrategy() = default;

    virtual bool start(FrameCallback callback) = 0;

    virtual void stop() = 0;
};

#endif // IGRABSTRATEGY_H
