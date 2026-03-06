#include "mymodel.h"

MyModel::MyModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_items << "Par.1" << "Par.2" << "Par.3" << "Par.4";
}

int MyModel::rowCount(const QModelIndex &parent) const
{
    return m_items.count();
}

QVariant MyModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()) return QVariant();

    //if(role == Qt::DisplayRole) return m_items.at(index.row());
    if(role == NameRole) return m_items.at(index.row());

    return QVariant();
}

void MyModel::addItem(const QString &text)
{
    beginInsertRows(QModelIndex(), m_items.count(), m_items.count());
    m_items << text;
    endInsertRows();
}

QHash<int, QByteArray> MyModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";   //  теперь в QML доступно свойство 'model.name'
    return roles;
}
