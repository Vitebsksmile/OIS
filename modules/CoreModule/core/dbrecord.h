#ifndef COREDBRECORD_H
#define COREDBRECORD_H

#include <QVariant>
#include <QVariantMap>

namespace Core {

struct DbOperationResult
{
    bool success = false;
    QString error;
    int insertedId = -1;
};

class DbRecord
{
public:
    DbRecord() = default;

    void insert(const QString &field, const QVariant &value);
    QVariant value(const QString &field) const;

    bool contains(const QString &field) const;
    bool isEmpty() const;

    const QVariantMap &values() const;

private:
    QVariantMap m_values;
    DbOperationResult m_info;
};

}

#endif // COREDBRECORD_H
