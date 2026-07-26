#ifndef IFRAMEOBSERVER_H
#define IFRAMEOBSERVER_H

<<<<<<< Updated upstream

=======
<<<<<<< Updated upstream:modules/CameraManagerModule/interface/IFrameObserver.h
>>>>>>> Stashed changes
#include "Frame.h"


class IFrameObserver
{
public:
    virtual ~IFrameObserver() = default;

    virtual void onFrameReady(
        OIS::Core::Frame frame
    ) = 0;
};
=======
>>>>>>> Stashed changes:modules/CameraManagerModule/private/IFrameObserver.h

#endif // IFRAMEOBSERVER_H
