#ifndef IFRAMEOBSERVER_H
#define IFRAMEOBSERVER_H

#include "Frame.h"


class IFrameObserver
{
public:
    virtual ~IFrameObserver() = default;

    virtual void onFrameReady(
        OIS::Core::Frame frame
    ) = 0;
};


#endif // IFRAMEOBSERVER_H
