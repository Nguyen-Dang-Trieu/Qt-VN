#include "Tbl_Vaultconfig.h"

VaultConfig::VaultConfig() { }

void VaultConfig::fields()
{
    table("vault_config");

    Id(id, "id", "INTEGER", "PRIMARY KEY AUTOINCREMENT");
    field(version,      "version",    "INTEGER", "NOT NULL");
    field(contentKey,   "Content_Key", "TEXT",    "NOT NULL");
    field(expirePeriod, "expire_period", "TEXT", "DEFAULT '30 days'");
}

void VaultConfig::fromMap(const QVariantMap &map)
{
    EZ_MAP(id,          "id");
    EZ_MAP(version,      "version");
    EZ_MAP(contentKey,   "Content_Key");
    EZ_MAP(expirePeriod, "expire_period");
}

bool VaultConfig::isValidId()
{
    // Kiểm tra bản ghi đã tồn tại hay chưa dựa trên ID hoặc tên Vault
    return expirePeriod > 0 || id > 0;
}
