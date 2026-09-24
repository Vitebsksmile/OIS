#ifndef USERREGISTRATIONMODEL_H
#define USERREGISTRATIONMODEL_H

#include <QAbstractTableModel>

class UserRegistrationModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit UserRegistrationModel(QObject *parent = nullptr) : QAbstractTableModel(parent) {}
    ~UserRegistrationModel() = default;
};

#endif // USERREGISTRATIONMODEL_H
