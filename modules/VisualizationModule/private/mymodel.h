#ifndef MYMODEL_H
#define MYMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QtQmlIntegration/qqmlintegration.h>

class MyModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT // Делаем доступным в QML как тип

public:
    //  определим имена ролей QML
    enum roleNames
    {
        NameRole = Qt::UserRole + 1    //  Пользовательские роли начинаются с этого числа
    };

    MyModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

      void addItem(const QString &text);

    virtual QHash<int, QByteArray> roleNames() const override;

private:
    QStringList m_items;
};


#endif // MYMODEL_H
