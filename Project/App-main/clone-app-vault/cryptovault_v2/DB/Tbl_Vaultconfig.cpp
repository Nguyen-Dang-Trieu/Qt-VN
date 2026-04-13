#include "Tbl_Vaultconfig.h"

VaultConfig::VaultConfig() { }

void VaultConfig::fields()
{
    table("vault_config");

    Id(id, "id", "INTEGER", "PRIMARY KEY AUTOINCREMENT");

    field(name, "vault_name", "TEXT",    "NOT NULL");
    field(version,   "version",    "INTEGER", "NOT NULL");
    field(masterKey, "master_key", "TEXT",    "NOT NULL");
    field(macKey,    "mac_key",    "TEXT",    "NOT NULL");
    field(expirePeriod, "expire_period", "TEXT", "DEFAULT '30 days'");
}

void VaultConfig::fromMap(const QVariantMap &map)
{
    EZ_MAP(id, "id");
    EZ_MAP(name, "vault_name");
    EZ_MAP(version, "version");
    EZ_MAP(masterKey, "master_key");
    EZ_MAP(macKey, "mac_key");
    EZ_MAP(expirePeriod, "expire_period");
}

bool VaultConfig::isValidId()
{
    // Kiểm tra bản ghi đã tồn tại hay chưa dựa trên ID hoặc tên Vault
    return expirePeriod > 0 || !name.isEmpty();
}
