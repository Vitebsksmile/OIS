#include "dbmodel.h"
#include <QSqlRelation>
#include <QSqlError>
#include <QDebug>

DbModel::DbModel(QObject *parent, QSqlDatabase db)
    : IDbModel(parent)
{
    m_tableDbModel = new QSqlRelationalTableModel(this, db);

    //  НАСТРОЙКА МАППИНГА СИГНАЛОВ:
    //  Когда m_tableDbModel говорит, что данные изменились, наш класс дублирует этот
    //  сигнал наружу.

    //  Сигнал изменения конкретных ячеек (например, отредактировали текст)
    // connect(m_tableDbModel, &QSqlTableModel::dataChanged,
    //         this, &ItemDbModel::dataChanged);

    //  Сигналы начала и конца добавления строк
    //  (чтобы QML плавно анимировал появление строк)
    // connect(m_tableDbModel, &QSqlTableModel::rowsAboutToBeInserted,
    //         this, &ItemDbModel::rowsAboutToBeInserted);
    // connect(m_tableDbModel, &QSqlTableModel::rowsInserted, this,
    //         &ItemDbModel::rowsInserted);

    //  Сигналы удаления строк
    // connect(m_tableDbModel, &QSqlTableModel::rowsAboutToBeRemoved,
    //         this, &ItemDbModel::rowsAboutToBeRemoved);
    // connect(m_tableDbModel, &QSqlTableModel::rowsRemoved,
    //         this, &ItemDbModel::rowsRemoved);

    //  Сигнал полной перезагрузки модели
    //  (например, при вызове select() или смене таблицы)
    // connect(m_tableDbModel, &QSqlTableModel::modelReset,
    //         this, &ItemDbModel::modelReset);

    //  Сигнал изменения структуры (изменился порядок строк или сортировка)
    // connect(m_tableDbModel, &QSqlTableModel::layoutChanged,
    //         this, &ItemDbModel::layoutChanged);
}

void DbModel::setTable(const QString &tableName)
{
    if (m_tableDbModel) {
        m_tableDbModel->setTable(tableName);

        //======= new -----
        //m_tableDbModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        //m_tableDbModel->select();
        //m_roles.clear();
        //QSqlRecord record = m_tableDbModel->record();
        //for (int i = 0; i < record.count(); i++) {
        //    m_roles.insert(Qt::UserRole + i + 1, record.fieldName(i).toUtf8());
        //}
        //======= new-end -----
    }
}

void DbModel::setRelation(int columnIdx,
                              const QString &foreignTable,
                              const QString &foreignId,
                              const QString &displayField
                              )
{
    if (m_tableDbModel) {
        // columnIdx   - индекс колонки в текущей таблице (например, 4-я колонка session_id в таблице boards)
        // foreignTable - имя связанной таблицы (например, "sessions")
        // foreignId    - ключевое поле в связанной таблице (например, "session_id")
        // displayField - поле, текст из которого мы хотим вывести на экран вместо ID (например, "started_at")
        m_tableDbModel->setRelation(columnIdx, QSqlRelation(foreignTable, foreignId, displayField));
    }
}

QAbstractItemModel *DbModel::dbModel()
{
    if (m_tableDbModel) {
        m_tableDbModel->select();
    }
    return m_tableDbModel;
}

QString DbModel::tableName() const
{
    return m_tableDbModel->tableName();
}

void DbModel::refreshData()
{
    if (m_tableDbModel) m_tableDbModel->select();
}

bool DbModel::isReadOnly() const
{
    return false;
}

void DbModel::saveChanges()
{
    if (m_tableDbModel) {
        if (!m_tableDbModel->submitAll()) {
            qCritical()
                << "Saved error relations data into SQLite:"
                << m_tableDbModel->lastError().text();
        }
    }
}
