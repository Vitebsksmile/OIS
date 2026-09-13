#ifndef IDBMODEL_H
#define IDBMODEL_H

#include <QObject>

class QAbstractItemModel;

class IDbModel : public QObject
{
    Q_OBJECT

public:
    explicit IDbModel(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~IDbModel() = default;

    virtual QAbstractItemModel* dbModel() = 0;
    virtual QString tableName() const = 0;
    //Added Row into Model
    //virtual void insertRow() = 0;
    virtual void refreshData() = 0;
    virtual bool isReadOnly() const = 0;
    virtual void saveChanges() = 0;

signals:
    //  Row addition signal
    void rowsInserted();
};

#endif // IDBMODEL_H
