#ifndef NODEDATA_H
#define NODEDATA_H

#include "ezsql.h"

class NodeData : public EzSql::BaseDBO
{
public:
    NodeData();

    qint64  id           = 0;
    quint64 node_id      = 0;
    quint64 bucket_id    = 0;
    QString slice_uuid   = "";
    quint64 slice_number = 0;


    void fields() override;
    void fromMap(const QVariantMap& map) override;
    bool isValidId() override;
};
#endif // NODEDATA_H
