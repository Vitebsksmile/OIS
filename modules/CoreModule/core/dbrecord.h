#ifndef COREDBRECORD_H
#define COREDBRECORD_H

#include <QVariant>
#include <QVariantMap>

namespace Core {

struct DbOperationResult
{
    bool success = false;
    QString error;
};

class DbRecord
{
public:
    DbRecord() = default;

    void setValue(const QString &field, const QVariant &value);
    QVariant value(const QString &field) const;

    bool contains(const QString &field) const;
    bool isEmpty() const;

    const QVariantMap& values() const;

private:
    QVariantMap m_values;
};

}

#endif // COREDBRECORD_H
