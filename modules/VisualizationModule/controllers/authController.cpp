#include "authController.h"

AuthController* AuthController::s_instance = nullptr;

AuthController::AuthController(VisualizationService *visualization,
                               QObject *parent)
    : m_visualization(visualization)
    , QObject(parent)
{
    s_instance = this;

    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    qDebug()
        << "AuthController: AuthController object created. Parent: "
        << parent;

    if (!m_visualization) {
        qWarning()
            << "WARNING! AuthController: AuthController object created without reference to facade";
    }
}

bool AuthController::login(const QString &username, const QString &password)
{

    if (username == "admin" && password == "1234") {
        emit authSuccess();
        return true;
    }
    emit authFailed("Invalid username or password!");
    return false;
}
