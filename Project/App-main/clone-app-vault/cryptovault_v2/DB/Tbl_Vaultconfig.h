#ifndef VAULTCONFIG_H
#define VAULTCONFIG_H

#include "ezsql.h"

class VaultConfig : public EzSql::BaseDBO
{
public:
    VaultConfig();

    qint64  id           = 0;
    quint32 version      = 0;
    QString contentKey   = "";
    QString expirePeriod = "";

    void fields() override;
    void fromMap(const QVariantMap& map) override;
    bool isValidId() override;
};

#endif // VAULTCONFIG_H
