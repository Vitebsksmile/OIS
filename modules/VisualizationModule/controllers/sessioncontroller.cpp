#include "sessioncontroller.h"
#include <QRegularExpression>
#include <QImage>

SessionController* SessionController::s_instance = nullptr;

SessionController::SessionController(VisualizationService *service,
                                     QObject *parent)
    : m_service(service)
    , QObject(parent)
{
    s_instance = this;

    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    qDebug() << "SessionController: SessionController object created. Parent:"
             << parent;

    if (!m_service) {
        qWarning() << "WARNING! SessionController: SessionController object created without reference to facade";
    }

    connect(this, &SessionController::triggered,
            m_service, &VisualizationService::onTriggered);

    connect(m_service, &VisualizationService::cropPCBReady,
            this, &SessionController::onCropPCBReady);

    connect(m_service, &VisualizationService::defectiveBoard,
            this, &SessionController::onDefectiveBoard);
}

void SessionController::startSession(const Core::AuthResult &result)
{
    m_currentSession = m_service->dbService()->creatSession(result);

    m_currentSession.username = result.username;
    m_currentSession.fullName = result.fullName;
}

void SessionController::finishCurrentSession()
{
    Core::SessionContext context = m_service->dbService()->finishSession(m_currentSession);
    if(!context.active) {
        qDebug() << "SessionController: session complete. context.active ="
                 << context.active;
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

void SessionController::toggleTrigger()
{
    //qDebug() << "SessionController::toggleTrigger()";
    emit triggered();
}

void SessionController::setCountFail()
{
    qDebug() << "SessionController::repairConfirmed(). m_countFail =" << m_countFail;
    m_countFail++;
    qDebug() << "SessionController::repairConfirmed(). m_countFail =" << m_countFail;
    emit countFailChanged();
}

QString SessionController::operatorName() const
{
    return m_currentSession.fullName;
}

int SessionController::countPass() const
{
    //qDebug() << "SessionController::countPCB(). m_countPCB =" << m_countTotal;
    return m_countPass;
}

void SessionController::setCountPass()
{
    m_countPass++;
    emit countPassChanged();
}

int SessionController::countFail() const
{
    return m_countFail;
}

void SessionController::onCropPCBReady(const QImage &cropPCB)
{
    if (cropPCB.isNull()) { return; }
    else {
        //qDebug() << "SessionController::onCropPCBReady(). m_countTotal =" << m_countTotal;
        m_countPass++;
        //qDebug() << "SessionController::onCropPCBReady(). m_countTotal =" << m_countTotal;
        emit countPassChanged();
    }
}

void SessionController::onDefectiveBoard()
{
    qDebug() << "SessionController::onDefectiveBoard()";
    emit defectiveBoard();
}
