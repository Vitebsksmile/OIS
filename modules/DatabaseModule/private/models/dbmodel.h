#ifndef DBMODEL_H
#define DBMODEL_H

//#include <QObject>
#include <QSqlDatabase>
//#include <QSqlRecord>
#include "IDbModel.h"

#include <QSqlRelationalTableModel>

class DbModel : public IDbModel
{
    Q_OBJECT

public:
    explicit DbModel(QObject *parent = nullptr,
                         QSqlDatabase db = QSqlDatabase()
                         );

    void setTable(const QString &tableName);

    void setRelation(int columnIdx,
                     const QString &foreignTable,
                     const QString &foreignId,
                     const QString &displayField
                     );

    //  Реализация методов интерфейса IItemModel
    QAbstractItemModel* dbModel() override;
    QString tableName() const override;
    //void insertRow() override;  //  new
    void refreshData() override;
    bool isReadOnly() const override;
    void saveChanges() override;

    //  Обязательное переопределение для QML (маппинг колонок базы в роли QML)
    //QHash<int, QByteArray> roleNames() const override
    //{
    //    QHash<int, QByteArray> roles;
    //    for (int i = 0; i < m_tableDbModel->columnCount(); i++) {
            //  Имя роли будет соответствовать названию колонки в БД
    //        roles.insert(Qt::UserRole + i + 1, headerData(i, Qt::Horizontal).toByteArray());
    //    }
    //    return roles;

        //======= new -----
        // if (m_roles.isEmpty()) {
        //     return QAbstractItemModel::roleNames();
        // }
        // return m_roles;
        //======= new-end -----
    //}

    // QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override
    // {
    //     if (role < Qt::UserRole) {
    //         return m_tableDbModel->data(index, role);
    //     }
    //     int columnIdx = role - Qt::UserRole - 1;
    //     return m_tableDbModel->data(m_tableDbModel->index(index.row(),
    //                                 columnIdx),
    //                                 Qt::DisplayRole);
    // }

    //  Перенаправляем чисто виртуальные методы QAbstractItemModel на QSqlTableModel
    // QModelIndex index(int row, int column, const QModelIndex &parent) const override
    // {
    //     return m_tableDbModel->index(row, column, parent);
    // }

    // QModelIndex parent(const QModelIndex& child) const override
    // {
    //     return QModelIndex();
    // }

    // int rowCount(const QModelIndex& parent = QModelIndex()) const override
    // {
    //     return m_tableDbModel->rowCount(parent);
    // }

    // int columnCount(const QModelIndex& parent) const override
    // {
    //     return m_tableDbModel->columnCount(parent);
    // }

private:
    QSqlRelationalTableModel* m_tableDbModel = nullptr;

    //======= new -----
    //QHash<int, QByteArray> m_roles;
    //======= new-end -----
};

#endif // DBMODEL_H
