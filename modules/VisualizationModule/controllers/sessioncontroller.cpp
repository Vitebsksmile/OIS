#include "sessioncontroller.h"
#include <QRegularExpression>

SessionController* SessionController::s_instance = nullptr;

SessionController::SessionController(VisualizationService *service,
                                     QObject *parent)
    : m_service(service)
    , QObject(parent)
{
    s_instance = this;

    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    qDebug()
        << "SessionController: SessionController object created. Parent: "
        << parent;

    if (!m_service) {
        qWarning()
            << "WARNING! SessionController: SessionController object created without reference to facade";
    }
}

bool SessionController::creatCamera(const QString &url)
{
    static const QRegularExpression re("^[.:]*$");

    if (url.isEmpty() || re.match(url).hasMatch()) {
        qCritical() << "SessionController: url is empty";
        emit errorOccurred("URL is empty");
        return false;
    }
    QString urlVideo = "http://" + url + "/video";
    qDebug() << "SessionController: url" << urlVideo;

    if (m_service->creatCamera(urlVideo)) {
        return true;
    } else {
        return false;
    }
}

bool SessionController::creatCamera(int cameraIndex)
{
    if (m_service->creatCamera(cameraIndex)) {
        return true;
    } else {
        return false;
    }
}

void SessionController::disconnectCamera()
{
    m_service->disconnectCamera();
}

void SessionController::readFrame(bool isRead)
{
    m_readFrame = isRead;
    m_service->procService()->readFrame(m_readFrame);
}

bool SessionController::isReadFrame()
{
    return m_readFrame;
}
