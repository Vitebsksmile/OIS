#ifndef IDBSQLRELATIONTABLEMODEL_H
#define IDBSQLRELATIONTABLEMODEL_H

#include <QObject>
#include <QSqlRelationalTableModel>
#include <QAbstractTableModel>

class IDbSqlRelationTableModel : public QSqlRelationalTableModel
{
    Q_OBJECT

public:
    explicit IDbSqlRelationTableModel(QSqlRelationalTableModel *parent = nullptr,
                                      const QSqlDatabase &db = QSqlDatabase())
        : QSqlRelationalTableModel(parent, db) {}

    virtual ~IDbSqlRelationTableModel() = default;

    virtual QAbstractTableModel* dbModel() = 0;

    void setRelation(int column, const QSqlRelation &relation) override;

    // Переопределяем роли для доступа к колонкам по их именам в QML
    QHash<int, QByteArray> roleNames() const override {
        QHash<int, QByteArray> roles;
        for (int i = 0; i < columnCount(); ++i) {
            roles.insert(Qt::UserRole + i + 1, headerData(i, Qt::Horizontal).toByteArray());
        }
        return roles;
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};

#endif // IDBSQLRELATIONTABLEMODEL_H
