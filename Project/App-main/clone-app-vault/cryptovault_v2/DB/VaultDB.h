#ifndef VAULTDB_H
#define VAULTDB_H

#include <QFile>
#include <QByteArray>

// DB
#include "ezsql.h"
#include "Tbl_Node.h"
#include "Tbl_Nodedata.h"
#include "Tbl_Del.h"
#include "Tbl_Vaultconfig.h"
#include "Tbl_Bucket.h"
#include "Tbl_Xattr.h"

class VaultDB {

public:
    EzSql::DbSet<Node>        node;     // Table ... operations
    EzSql::DbSet<Bucket>      bucket;
    EzSql::DbSet<VaultConfig> vaultConfig;
    EzSql::DbSet<DelNode>     delNode;
    EzSql::DbSet<NodeData>    nodeData;
    EzSql::DbSet<XAttr>       xattr;

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

    // QString deriveDbKey(const QString& dbPath, const QString& userPassword);

private:

    QString currentDBPath;
};
#endif // VAULTDB_H
