#include "dbmodelcontroller.h"

DbModelController* DbModelController::s_instance = nullptr;

DbModelController::DbModelController(VisualizationService *visualization,
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

QAbstractTableModel* DbModelController::abstractTableModel(const QString &tableName)
{
    if (m_abstractModelsMap.contains(tableName)) {
        qDebug() << "DbModelController: The model already exists";
        return m_abstractModelsMap.value(tableName);
    }
    qDebug() << "DbModelController: The model does not yet exists";
    //QAbstractTableModel *abstractModelsMap = m_dbService->abstractTableModel(tableName);    //  ---
    QAbstractTableModel *abstractModel = m_visualization->dbService()->abstractTableModel(tableName);
    //m_abstractModelsMap.insert(tableName, abstractModelsMap);   //  ---
    m_abstractModelsMap.insert(tableName, abstractModel);
    return m_abstractModelsMap.value(tableName);
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

    for (int row = 0; row < rows; ++row) {
        qDebug() << QString("--- Запись %1 ---").arg(row);

        // В первой колонке (индекс 0) запрашиваем данные по всем кастомным ролям
        QModelIndex index = model->index(row, 0);

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
