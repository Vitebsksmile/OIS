#include "authController.h"
#include "dbrecord.h"

AuthController* AuthController::s_instance = nullptr;

AuthController::AuthController(VisualizationService *service,
                               QObject *parent)
    : m_service(service)
    , QObject(parent)
{
    s_instance = this;

    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    qDebug()
        << "AuthController: AuthController object created. Parent: "
        << parent;

    if (!m_service) {
        qWarning()
            << "WARNING! AuthController: AuthController object created without reference to facade";
    }
}

void AuthController::setDbController(DbModelController *dbController)
{
    m_dbController = dbController;
}

bool AuthController::existsByLogin(const QString &username)
{
    //QAbstractTableModel *model = m_serviceModule->dbController()
    QAbstractTableModel *model = m_service->dbController()->abstractTableModel("operators");
    int columnCount = model->columnCount();
    while(columnCount) {
        QVariant header = model->headerData(columnCount, Qt::Horizontal);
        if (header.toString() == "username") {
            int rowCount = model->rowCount();
            for (int row = 0; row < rowCount; row++) {
                QModelIndex index = model->index(row, columnCount);
                QVariant value = model->data(index);
                if (value.toString() == username) {
                    m_identificationRow = row;
                    m_username = value.toString();
                    return true;
                }
            }
        }
        columnCount--;
    }
    emit authFailed("Incorrect username!");
    return false;
}

bool AuthController::login(const QString &username, const QString &password)
{
    m_authResult = m_service->dbService()->authenticate(username, password);
    if (m_authResult.success) {
        this->creatSession();
        emit authenticationSuccess();
        return true;
    } else {
        emit authFailed("Incorrect password!", m_authResult.error);
        return false;
    }
}

bool AuthController::isUsernameUnique(const QString &username)
{
    QAbstractTableModel *model = m_dbController->abstractTableModel("operators");
    int columnCount = model->columnCount();
    while(columnCount) {
        QVariant header = model->headerData(columnCount, Qt::Horizontal);
        if (header.toString() == "username") {
            int rowCount = model->rowCount();
            for (int row = 0; row < rowCount; row++) {
                QModelIndex index = model->index(row, columnCount);
                QVariant value = model->data(index);
                if (value.toString() == username) {
                    emit authFailed("The username is not unique!");
                    qDebug() << "AuthController: The username is not unique!";
                    return false;
                }
            }
        }
        columnCount--;
    }
    return true;
}

bool AuthController::registerUser(const QString &username,
                                  const QString &fullName,
                                  const QString &password)
{
    Core::DbOperationResult result = m_service->dbService()->creatUser(username,
                                                                       password,
                                                                       fullName,
                                                                       "operator",
                                                                       "New user registration");
    if (!result.success) {
        authFailed("Registration failed!", result.error);
        qCritical() << "AuthController: Registration failed!" + result.error;
        return false;
    } else {
        authFailed("Registration was successful!", "");
        return true;
    }
}

QString AuthController::operatorName() const
{
    return m_authResult.fullName;
}

bool AuthController::creatSession()
{
    m_sessionContext = m_service->dbService()->creatSession(m_authResult);

    m_sessionContext.sessionId = m_authResult.operatorId;
    m_sessionContext.username = m_authResult.username;
    m_sessionContext.fullName = m_authResult.fullName;

    return true;
}
