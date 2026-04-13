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
#define BUCKET_SIZE       (16  * 1024)                  // 16 KiB
#define CHUNK_SIZE        (32)                          // 32 Bytes
#define SLICE_SIZE        (CHUNK_SIZE + 28)             // 60 Byte
#define SLICES_PER_BUCKET (BUCKET_SIZE / SLICE_SIZE)


class VaultController : public QObject {
    Q_OBJECT
public:
    explicit VaultController(QObject *parent = nullptr);

    Q_INVOKABLE bool createVaultFolder(const QString &vaultName, const QString &password,  const QString &path);
    Q_INVOKABLE bool importDataToVault(QString sourcePath, QString vaultName, QString password);
    Q_INVOKABLE bool exportFileFromVault(int fileId, QString destPath, QString fileName);

    Q_INVOKABLE QVariantList loadVaults();
    Q_INVOKABLE bool deleteVault(const QString &vaultPath);
    Q_INVOKABLE QString renameVault(const QString &oldPath, const QString &newName);

private:
    CPathDB *pathDB; // DB is used to save path of Vault Folder

private:
    bool copyRecursively(const QString &srcFilePath, const QString &targetFilePath, VaultDB &vDB, int parentId);
    bool ingestFileToBuckets(VaultDB &vDB, quint64 nodeID, QString pathSrcFile, QString pathOfVaultFolder, QString password);
    quint64 ensureAvailableBucket(VaultDB &vDB, QString pathOfVaultFolder);
};

#endif // VAULTCONTROLLER_H
