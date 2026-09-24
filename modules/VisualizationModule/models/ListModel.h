#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QAbstractListModel>

class ListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum StringRoles {
        DisplayRole = Qt::DisplayRole,
        EditRole = Qt::EditRole
    };

    explicit ListModel(QObject *parent = nullptr) : QAbstractListModel(parent) {}
    ~ListModel() = default;

    void appendRow(const QString &row)
    {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        rows << row;
        endInsertRows();
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        if (parent.isValid()) { return 0; }
        return rows.count();
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || index.row() >= rows.count()) {
            return QVariant();
        }

        if (role == Qt::DisplayRole || role == Qt::EditRole) {
            return rows[index.row()];
        }
        return QVariant();
    }

    QHash<int, QByteArray> roleNames() const override{
        QHash<int, QByteArray> roles;
        roles[DisplayRole] = "display";
        roles[EditRole] = "edit";
        return roles;
    }

private:
    QList<QVariant> rows;

};

#endif // LISTMODEL_H
