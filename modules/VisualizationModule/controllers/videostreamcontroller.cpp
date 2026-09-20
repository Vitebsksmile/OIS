#include "videostreamcontroller.h"

VideoStreamController* VideoStreamController::s_instance = nullptr;



VideoStreamController::VideoStreamController(VisualizationService *visualization,
                                             QObject *parent)
    : m_visualization(visualization)
    , QObject(parent)
{
    s_instance = this;

    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    qDebug()
        << "VideoStreamController: VideoStreamController object created. Parent: "
        << parent;

    if (!m_visualization) {
        qWarning()
            << "WARNING! VideoStreamController: VideoStreamController object created without reference to facade";
    }
}

void VideoStreamController::registerProvider(VideoProvider *provider)
{
    if (provider && !m_providers.contains(provider)) {
        qDebug()
            << "VideoStreamController: VideoStreamController received the provider object:"
            << provider;

        m_providers.append(provider);

        QString frameSource = provider->frameSource();
        if (frameSource == "camera") {
            qDebug()
                << "VideoStreamController: provider ="
                << provider
                << "provider->frameSource ="
                << provider->frameSource();

            connect(m_visualization, &VisualizationService::frameReady
                    , provider, &VideoProvider::onFrameReady);
        }
        if (frameSource == "processor") {
            connect(m_visualization, &VisualizationService::frameWithBoxesReady
                    , provider, &VideoProvider::onFrameWithBoxesReady);
        }

    }
}
