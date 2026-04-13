#include "Tbl_PathVault.h"

CSavedVault::CSavedVault() {}

void CSavedVault::fields()
{
    table("User_InfoVaults");

    Id(id, "id", "INTEGER", "PRIMARY KEY AUTOINCREMENT");
    field(name, "name", "TEXT", "UNIQUE NOT NULL");
    field(path, "path", "TEXT", "UNIQUE NOT NULL");
}

void CSavedVault::fromMap(const QVariantMap& map)
{
    EZ_MAP(id, "id");
    EZ_MAP(name, "name");
    EZ_MAP(path, "path");
}

bool CSavedVault::isValidId()
{
    return id > 0;
}
