#ifndef BUCKET_H
#define BUCKET_H

#include "ezsql.h"

class Bucket : public EzSql::BaseDBO {

public:
    quint64    id;
    QString    relative_path;
    QByteArray slices;
    bool       is_full;

public:
    Bucket();
    void fields() override;
    void fromMap(const QVariantMap& map) override;
    bool isValidId() override;
};

#endif // BUCKET_H
