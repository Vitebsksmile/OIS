#include "videostreamcontroller.h"

VideoStreamController* VideoStreamController::s_instance = nullptr;



VideoStreamController::VideoStreamController(VisualizationService *service,
                                             QObject *parent)
    : m_service(service)
    , QObject(parent)
{
    s_instance = this;

    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    qDebug()
        << "VideoStreamController: VideoStreamController object created. Parent: "
        << parent;

    if (!m_service) {
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
            connect(m_service, &VisualizationService::rawImageFrameReady
                    , provider, &VideoProvider::onRawImageFrameReady);
        }
        if (frameSource == "processor") {
            connect(m_service, &VisualizationService::processedFrameReady
                    , provider, &VideoProvider::onProcessedFrameReady);
        }
        if (frameSource == "object") {
            connect(m_service, &VisualizationService::frameWithBoxesReady
                    , provider, &VideoProvider::onFrameWithBoxesReady);
        }
    }
}
