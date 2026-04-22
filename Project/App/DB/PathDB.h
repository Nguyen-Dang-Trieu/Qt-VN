#ifndef PATHDB_H
#define PATHDB_H

#include <QObject>
#include <QFile>
#include <QByteArray>
#include <QVariantList>

#include "Tbl_PathVault.h"


class CPathDB : public QObject {
    Q_OBJECT

public:
    EzSql::DbSet<CSavedVault> User_InfoVaults; // ops of User_InfoVaults

public:
    explicit CPathDB(QObject *parent = nullptr);
    ~CPathDB();

    bool open(const QString& dbPath);
    void close();
    bool migrate();
    sqlite3* connection() const { return _db_.connection(); }

    // Operations for User_InfoVaults table
    QString getVaultPathByName(const QString &name);

    bool saveVaultFolder(const QString &name, const QString &nameDB, const QString &path);
    QVariantList loadVaultFolders(void);
    bool deleteVaultFolder(const QString &vaultFullPath);
    bool updateVaultFolder(const QString &oldPath, const QString &newPath);

    // add start 20260415
    void setUsernameSignup(const QString &username);
    void setPasswordSignup(const QString &password);
    QString getUsernameSignup() const;
    QString getPasswordSignup() const;
    QString getKEK() const;
    // add end 20260415

    // add start 20260420
    bool openWithKEK(const QString& dbPath, const QString& kekHex);
    bool changePassword(const QString& newPassword);
    // add end 20260420
    QString getNameDB(const QString &name);

private:
    bool initDB();
    bool migrateTo(int version);
    bool migrateV1ToV2() { return true; }

    QString deriveDbKey(const QString& dbPath);
    void createPathDB(void);

private:
    EzSql::DataBase _db_;
    QString currentDBPath;  // Duong dan toi DB quan li toan bo Vault Folder cua User

    // add start 20260415
    QString m_usernameSignup;
    QString m_passwordSignup;
    QString m_derivedKey;
    // add end 20260415

};

#endif // PATHDB_HV

/**
  Table:


**/
