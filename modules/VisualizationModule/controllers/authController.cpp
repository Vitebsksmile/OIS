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

void AuthController::setDbController(DbModelController *dbController)
{
    m_dbController = dbController;
}

bool AuthController::identification(const QString &username)
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
                    m_identificationRow = row;
                    m_username = value.toString();
                    return true;
                }
            }
        }
        columnCount--;
    }
    emit authFailed("Invalid username!");
    return false;
}

bool AuthController::autentification(const QString &username, const QString &password)
{
    QAbstractTableModel *model = m_dbController->abstractTableModel("operators");

    int columnCount = model->columnCount();
    if (columnCount == -1) { return false; }
    if (m_username != username) { return false; }
    while(columnCount) {
        QVariant header = model->headerData(columnCount, Qt::Horizontal);
        if (header.toString() == "password") {
            QModelIndex index = model->index(m_identificationRow, columnCount);
            QVariant value = model->data(index);
            if (value.toString() == password) {
                emit authSuccess();
                return true;
            }
        }
        columnCount--;
    }
    emit authFailed("Invalid username or password!");
    return false;
}
