#include "dbrecord.h"

namespace Core {

void Core::DbRecord::setValue(const QString &field, const QVariant &value)
{
    m_values.insert(field, value);
}

QVariant DbRecord::value(const QString &field) const
{
    return m_values.value(field);
}

bool DbRecord::contains(const QString &field) const
{
    return m_values.contains(field);
}

bool DbRecord::isEmpty() const
{
    return m_values.isEmpty();
}

const QVariantMap &DbRecord::values() const
{
    return m_values;
}

}
