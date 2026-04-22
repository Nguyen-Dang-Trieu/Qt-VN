#include "Tbl_Bucket.h"

Bucket::Bucket() {}

void Bucket::fields()
{
    // Name table in DB
    table("bucket");

    // Id(variable, column_name, type, constraints)
    Id(id, "id", "INTEGER", "PRIMARY KEY AUTOINCREMENT");

    // field(variable, column_name, constraints)
    field(relative_path, "relative_path", "TEXT UNIQUE");
    field(slices,        "slices",        "BLOB NOT NULL");
    field(is_full,       "is_full",       "INTEGER DEFAULT 0"); // 0 = false, 1 = true
}

void Bucket::fromMap(const QVariantMap& map)
{
    EZ_MAP(id,            "id");
    EZ_MAP(relative_path, "relative_path");
    EZ_MAP(slices,        "slices");
    EZ_MAP(is_full,       "is_full");
}

bool Bucket::isValidId()
{
    return id > 0;
}
