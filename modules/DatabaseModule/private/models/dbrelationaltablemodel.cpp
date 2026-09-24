#include "dbrelationaltablemodel.h"
#include <QSqlRecord>
#include <QSqlError>


DbRelationalTableModel::DbRelationalTableModel(QObject *parent, const QSqlDatabase &db)
    : QSqlRelationalTableModel(parent, db)
{
    setEditStrategy(QSqlTableModel::OnManualSubmit);
}

void DbRelationalTableModel::setTable(const QString &tableName)
{
    if (tableName.isEmpty()) {
        qCritical() << "DbRelationalTableModel: Table name is empty";
        return;
    }
    QSqlRelationalTableModel::setTable(tableName);
}

const QString &DbRelationalTableModel::tableName() const
{
    return m_tableName;
}

QVariant DbRelationalTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) { return {}; }
    if (role >= Qt::UserRole) {
        const int column = role - Qt::UserRole;
        if (column < 0 || column >= this->columnCount()) { return {}; }
        return QSqlRelationalTableModel::data(this->index(index.row(), column), Qt::DisplayRole);
    }
    return QSqlRelationalTableModel::data(index, role);

    // switch (role) {
    // case Qt::DisplayRole:
    //     return QString("%1, %2").arg(index.column()).arg(index.row());
    // default:
    //     break;
    // }
    // return QVariant();
}

QHash<int, QByteArray> DbRelationalTableModel::roleNames() const
{
    // QHash<int, QByteArray> roles;
    // for (int column = 0; column < this->columnCount(); ++column) {
    //     const QString fieldName = this->record().fieldName(column);
    //     roles.insert(Qt::UserRole + column, fieldName.toUtf8());
    // }
    // return roles;

    return { {Qt::DisplayRole, "display"} };
}


