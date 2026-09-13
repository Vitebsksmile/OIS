#include "dbmodelcontroller.h"

DbModelController* DbModelController::s_instance = nullptr;

DbModelController::DbModelController(IVisualizationService *visualization,
                                     QObject *parent)
    : m_visualization(visualization)
    , QObject(parent)
{
    s_instance = this;

    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    qDebug()
        << "DbModelController: DbModelController object created. Parent: "
        << parent;

    if (!m_visualization) {
        qWarning()
            << "WARNING! DbModelController: DbModelController object created without reference to facade";
    }
}

void DbModelController::registerProvider(VideoProvider *provider)
{
    if (provider && !m_providers.contains(provider)) {
        qDebug()
            << "DbModelController: DbModelController received the provider object:"
            << provider;

        m_providers.append(provider);

        connect(m_visualization, &IVisualizationService::frameWithBoxesReady
                , provider, &VideoProvider::onFrameWithBoxesReady);
    }
}

void DbModelController::setDbService(IDatabaseService *dbService)
{
    m_dbService = dbService;
    qDebug()
        << "DbModelController: m_dbService ="
        << m_dbService;

    this->populateModelsMap();
}

QAbstractItemModel* DbModelController::dbModel(const QString &tableName) const
{
    if (m_dbModelMap.contains(tableName)) {
        auto modelPointer = m_dbModelMap[tableName];

        if (!modelPointer.isNull()) {
            return modelPointer->dbModel();
        }
    }
    return nullptr;
}

QAbstractItemModel *DbModelController::currentModel() const
{
    return m_currentModel;
}

void DbModelController::setOperatorsModel(IDbModel *dbModel)
{
    m_dbModel.reset(dbModel);
}

void DbModelController::updateModel(IDbModel *dbModel)
{
    m_dbModel.reset(dbModel);

    //  Пинаем QML, чтобы данные обновились
    emit operatorsModelChanged();
}

void DbModelController::populateModelsMap()
{
    QStringList tables = m_dbService->availableTables();
    m_dbModelMap.clear();

    for (const QString &tableName : tables) {
        IDbModel* dbModel = m_dbService->model(tableName);

        if (dbModel) {
            m_dbModelMap.insert(tableName, QPointer<IDbModel>(dbModel));
        } else {
            qCritical() << "DbModelController: Модель пуста!";
        }
    }
}

void DbModelController::logAbstractModel(QAbstractItemModel *model)
{
    if (!model) {
        qDebug() << "❌ Модель пуста (nullptr)";
        return;
    }

    int rows = model->rowCount();
    int cols = model->columnCount();

    qDebug() << "================ LOGGING MODEL ================";
    qDebug() << "Размер таблицы:" << rows << "строк(и)," << cols << "колонок(ки)";

    // Проходим циклом по каждой строке
    for (int r = 0; r < rows; ++r) {
        QString rowText = QString("Строка %1: ").arg(r);

        // Проходим циклом по каждой колонке в этой строке
        for (int c = 0; c < cols; ++c) {
            // 1. Получаем индекс ячейки (строка r, колонка c)
            QModelIndex index = model->index(r, c);

            // 2. Достаем данные по стандартной роли Qt::DisplayRole (текст для отображения)
            QVariant data = model->data(index, Qt::DisplayRole);

            rowText += QString("[%1] ").arg(data.toString());
        }

        // Выводим собранную строку в лог
        qDebug() << rowText.toUtf8().constData();
    }
    qDebug() << "===============================================";
}

void DbModelController::logQmlModelRoles(QAbstractItemModel *model)
{
    if (!model) return;

    // Получаем список всех ролей, зарегистрированных в модели
    QHash<int, QByteArray> roles = model->roleNames();
    int rows = model->rowCount();

    qDebug() << "================ LOGGING QML ROLES ================";

    for (int r = 0; r < rows; ++r) {
        qDebug() << QString("--- Запись %1 ---").arg(r);

        // В первой колонке (индекс 0) запрашиваем данные по всем кастомным ролям
        QModelIndex index = model->index(r, 0);

        QHashIterator<int, QByteArray> it(roles);
        while (it.hasNext()) {
            it.next();
            int roleType = it.key();
            QString roleName = QString::fromUtf8(it.value());

            // Достаем значение для конкретной роли
            QVariant value = model->data(index, roleType);

            qDebug() << QString("  %1: %2").arg(roleName, value.toString());
        }
    }
    qDebug() << "==================================================";
}
