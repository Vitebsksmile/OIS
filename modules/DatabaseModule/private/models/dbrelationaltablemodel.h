#ifndef DBRELATIONALTABLEMODEL_H
#define DBRELATIONALTABLEMODEL_H

#include <QSqlRelationalTableModel>

class DbRelationalTableModel : public QSqlRelationalTableModel
{
    Q_OBJECT

public:
    explicit DbRelationalTableModel(QObject *parent = nullptr,
                                    const QSqlDatabase &db = QSqlDatabase());
    ~DbRelationalTableModel() = default;

    //  For DatabaseService
    //-bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    //-*bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    //-bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    //-bool submitAll();       //  public slot QSqlTableModel
    //-void revertAll();       //  public slot QSqlTableModel
    //-bool select() override; // QSqlQueryModel / ??? нужен ли

    //  Convenience
    void setTable(const QString &tableName) override;
    //-void setRelational(int columnIdx, const QString &foreignTable, const QString &foreignId, const QString &displayField);

    const QString& tableName() const;
    //  clearModel();
    //  lastError();

    //  For VisualizationModule
    //-int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    //-int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    //-QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QString m_tableName;
    QHash<int, QByteArray> m_roleNames;
};

#endif // DBRELATIONALTABLEMODEL_H
