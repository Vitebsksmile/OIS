#include "videostreamservice.h"

VideoStreamService* VideoStreamService::s_instance = nullptr;



VideoStreamService::VideoStreamService(VisualizationService *visualization,
                                       QObject *parent)
    : m_visualization(visualization)
    , QObject(parent)
{
    s_instance = this;

    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    qDebug()
        << "VideoStreamService: VideoStreamService object created. Parent: "
        << parent;

    if (!m_visualization) {
        qWarning()
        << "WARNING! VideoStreamService: VideoStreamService object created without reference to facade";
    }
}

void VideoStreamService::registerProvider(VideoProvider *provider)
{
    if (provider && !m_providers.contains(provider)) {
        qDebug()
        << "VideoStreamService: VideoStreamService received the provider object:"
        << provider;

        m_providers.append(provider);

        connect(m_visualization, &VisualizationService::frameReady
                , provider, &VideoProvider::onFrameReady);
    }
}
