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

void AuthController::setDbController(DbModelController *dbController)
{
    m_dbController = dbController;
}

bool AuthController::existsByLogin(const QString &username)
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

bool AuthController::authenticate(const QString &username, const QString &password)
{
    QAbstractTableModel *model = m_dbController->abstractTableModel("operators");
    int columnCount = model->columnCount();
    if (columnCount == -1) { return false; }
    if (!this->existsByLogin(username)) { return false; }
    while(columnCount) {
        QVariant header = model->headerData(columnCount, Qt::Horizontal);
        if (header.toString() == "password") {
            QModelIndex index = model->index(m_identificationRow, columnCount);
            QVariant value = model->data(index);
            if (value.toString() == password) {
                emit authenticationSuccess();
                m_isAuthorizated = true;
                return true;
            }
        }
        columnCount--;
    }
    emit authFailed("Invalid password!");
    return false;
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

bool AuthController::isAuthorizated()
{
    return m_isAuthorizated;
}

QStandardItemModel *AuthController::registrationModel()
{
    QAbstractTableModel *model = m_dbController->abstractTableModel("operators");
    int columnCount = model->columnCount();
    QStringList headers;
    QStandardItemModel *userRegistrationModel = new QStandardItemModel(this);
    userRegistrationModel->setRowCount(columnCount - 1);
    if (columnCount == -1) { return nullptr; }
    for (int col = 1; col < columnCount; col++) {
        QVariant header = model->headerData(col, Qt::Horizontal);
        userRegistrationModel->setHeaderData(col - 1, Qt::Vertical, header, Qt::DisplayRole);
        headers << header.toString();
    }
    userRegistrationModel->setColumnCount(1);

    // Заполняем модель тестовыми данными
    for (int row = 0; row < headers.count(); row++) {
        QModelIndex index = userRegistrationModel->index(row, 0);
        if (headers[row].contains("username")) {
            userRegistrationModel->setData(index, "Come up with a username", Qt::DisplayRole);
        }
        if (headers[row].contains("full_name")) {
            userRegistrationModel->setData(index, "Enter your full name", Qt::DisplayRole);
        }
        if (headers[row].contains("password")) {
            userRegistrationModel->setData(index, "Create a password", Qt::DisplayRole);
        }
    }

    return userRegistrationModel;
}

ListModel* AuthController::userRegistrationModel()
{
    QAbstractTableModel *model = m_dbController->abstractTableModel("operators");
    int columnCount = model->columnCount();

    if (columnCount == -1) { return nullptr; }

    ListModel *userRegistrationModel = new ListModel(this);

    for (int col = 1; col < columnCount; col++) {
        QVariant header = model->headerData(columnCount, Qt::Horizontal);
        userRegistrationModel->appendRow(header.toString());
    }

    return userRegistrationModel;
}
