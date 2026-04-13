#ifndef VAULTCONFIG_H
#define VAULTCONFIG_H

#include "ezsql.h"

class VaultConfig : public EzSql::BaseDBO
{
public:
    VaultConfig();

    qint64  id           = 0;
    QString name    = "";
    quint32 version      = 0;
    QString masterKey    = "";
    QString macKey       = "";
    QString expirePeriod = "";

    void fields() override;
    void fromMap(const QVariantMap& map) override;
    bool isValidId() override;
};

#endif // VAULTCONFIG_H
