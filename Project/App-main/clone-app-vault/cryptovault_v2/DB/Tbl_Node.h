#ifndef NODE_H
#define NODE_H

#include "ezsql.h"

class Node : public EzSql::BaseDBO {

public:
    enum Type
    {
        Dir = 1,
        File
    };

public:
    qint64  m_id     = 0;
    qint64  m_type   = 0;
    qint64  m_mode   = 0;
    qint64  m_atime  = 0;
    qint64  m_time   = 0;
    quint64 m_length = 0;
    QString m_name   = "";
    qint64  m_parent = 0;

public:
    Node();
    void fields() override;
    void fromMap(const QVariantMap& map) override;
    bool isValidId() override;
};

#endif
