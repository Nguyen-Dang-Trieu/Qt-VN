#ifndef XATTR_H
#define XATTR_H

#include "ezsql.h"

class XAttr : public EzSql::BaseDBO
{
public:
    XAttr();

    qint64     xattr_id     = 0;
    quint64    xattr_nodeId = 0;
    QString    xattr_name   = "";
    QByteArray xattr_value;

    void fields() override;
    void fromMap(const QVariantMap& map) override;
    bool isValidId() override;
};

#endif // XATTR_H
