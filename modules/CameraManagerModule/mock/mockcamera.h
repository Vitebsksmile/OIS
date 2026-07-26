#ifndef MOCKCAMERA_H
#define MOCKCAMERA_H

#include <QObject>
#include <QTimer>

#include "CameraTypes.h"
#include "ICamera.h"


class IFrameObserver;

class MockCamera final
    : public QObject,
      public ICamera
{
    Q_OBJECT

public:

    explicit MockCamera(
        OIS::Core::CameraConfig config,
        QObject* parent = nullptr
        );

    ~MockCamera() override;

    bool open() override;

    void close() override;

    bool startGrab() override;

    void stopGrab() override;

    void setObserver(IFrameObserver* observer) override;

    OIS::Core::CameraState state() const override;

private slots:

    void generateFrame();

private:

    OIS::Core::CameraConfig m_config;

    OIS::Core::CameraState m_state{ OIS::Core::CameraState::Disconnected };

    IFrameObserver* m_observer{ nullptr };

    QTimer* m_timer{ nullptr };

    std::uint64_t m_frameNumber{ 0 };
};

#endif // MOCKCAMERA_H
