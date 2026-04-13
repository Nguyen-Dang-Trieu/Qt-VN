#ifndef VAULTDB_H
#define VAULTDB_H

#include <QFile>
#include <QByteArray>
#include "hkdf.h"
#include "scrypt.h"

// DB
#include "ezsql.h"
#include "node.h"
#include "nodedata.h"
#include "del.h"
#include "logger.h"
#include "vaultconfig.h"
#include "bucket.h"
#include "xattr.h"

class VaultDB {

public:
    EzSql::DbSet<Node> node; // Table node operations
    EzSql::DataBase _db_;

public:
    VaultDB();
    ~VaultDB();

    bool open(const QString& dbPath, const QString& userPassword);
    void close();
    bool migrate();
    sqlite3* connection() const { return _db_.connection(); }

private:
    bool initDB();
    bool migrateTo(int version);
    bool migrateV1ToV2() {return true;};

    QString deriveDbKey(const QString& dbPath, const QString& userPassword);

private:

    QString currentDBPath;
};
#endif // VAULTDB_H
