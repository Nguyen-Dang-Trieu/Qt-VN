#ifndef DEL_H
#define DEL_H

#include "ezsql.h"

class DelNode : public EzSql::BaseDBO
{
public:
    DelNode();

    quint64 m_inode = 0;
    quint64 m_expireAt = 0;

    void fields() override;
    void fromMap(const QVariantMap& map) override;
    bool isValidId() override;
};

class DelSlices : public EzSql::BaseDBO
{
public:
    DelSlices();

    QString m_sliceUuid = "";
    quint64 m_bucketId = 0;

    void fields() override;
    void fromMap(const QVariantMap& map) override;
    bool isValidId() override;
};
#endif // DEL_H
