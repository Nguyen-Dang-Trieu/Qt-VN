#ifndef TBL_PATHVAULT_H
#define TBL_PATHVAULT_H

#include "ezsql.h"

class CSavedVault : public EzSql::BaseDBO {

public:
    qint64  id = 0;
    QString name = "";
    QString nameDB = "";
    QString path = "";

public:
    CSavedVault();
    void fields() override;
    void fromMap(const QVariantMap& map) override;
    bool isValidId() override;
};

#endif // TBL_PATHVAULT_H
