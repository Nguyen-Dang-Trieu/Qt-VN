#ifndef VAULTCONTROLLER_H
#define VAULTCONTROLLER_H

#include <QObject>
#include <QUrl>
#include <QDir>
#include <QDebug>
#include <QFileInfo>

#include "VaultDB.h"
#include "PathDB.h"

//#define BUCKET_SIZE (128 * 1024 * 1024) // 128 MiB
//#define CHUNK_SIZE  (32 * 1024) // 32 KiB
//#define SLICE_SIZE  CHUNK_SIZE + 12 + 16 // (ciphertext + IV + Tag
//#define SLICES_PER_BUCKET (BUCKET_SIZE / SLICE_SIZE)

// Scale
#define BUCKET_SIZE       (1024 * 1024)                 // 1 MiB  - 1.048.576 Bytes
#define CHUNK_SIZE        (32 * 1024)                   // 32 KiB - 32.768 Bytes
#define SLICE_SIZE        (CHUNK_SIZE + 28)             // CHUNK_SIZE + 12 IV + 16 Tag
#define SLICES_PER_BUCKET (BUCKET_SIZE / SLICE_SIZE)    // ~16 slices mỗi bucket


class VaultController : public QObject {
    Q_OBJECT

signals:
    // Signal phát ra khi Vault được mở thành công (hoặc khi cần truyền data cho Model)
    void vaultOpened(const QString &vaultName, const QString &passVault, const QString &pathDB);

public:
    explicit VaultController(QObject *parent = nullptr);

    Q_INVOKABLE bool openVault(const QString &vaultName, const QString &password, const QString &path); // NEW

    Q_INVOKABLE bool createVaultFolder(const QString &vaultName, const QString &password,  const QString &path);
    Q_INVOKABLE bool importDataToVault(QString sourcePath, QString vaultName, QString password);
    Q_INVOKABLE bool exportFileFromVault(QString destPath, QString vaultName, QString vaultPass, int fileId, QString fileName);

    Q_INVOKABLE QVariantList loadVaults();
    Q_INVOKABLE bool deleteVault(const QString &vaultPath);
    Q_INVOKABLE QString renameVault(const QString &oldPath, const QString &newNameVault);

private:
    CPathDB *pathDB; // DB is used to save path of Vault Folder

private:
    bool copyRecursively(const QString &srcFilePath, const QString &targetFilePath, VaultDB &vDB, int parentId, const QString &basePath);
    bool ingestFileToBuckets(VaultDB &vDB, quint64 nodeID, QString pathSrcFile, QString pathOfVaultFolder);
    quint64 ensureAvailableBucket(VaultDB &vDB, QString pathOfVaultFolder);
};

#endif // VAULTCONTROLLER_H
