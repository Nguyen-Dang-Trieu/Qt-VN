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
#define CHUNK_SIZE        (64 * 1024)                   // 64 KiB - 65.536 Bytes
#define SLICE_SIZE        (CHUNK_SIZE + 28)
#define SLICES_PER_BUCKET (BUCKET_SIZE / SLICE_SIZE)    // ~16 slices mỗi bucket
#define HEADER_FILE       (68)                          // 12 IV + 40 Bytes (8 bytes 0xFF + 32 bytes encrypted ContentKey ) + 16 Tag


class VaultController : public QObject {
    Q_OBJECT

signals:
    // Signal phát ra khi Vault được mở thành công (hoặc khi cần truyền data cho Model)
    void vaultOpened(const QString &dbPath, const QString &password); // NEW

public:
    explicit VaultController(QObject *parent = nullptr);

    Q_INVOKABLE bool openVault(const QString &vaultName, const QString &password, const QString &path); // NEW

    Q_INVOKABLE bool createVaultFolder(const QString &vaultName, const QString &password,  const QString &path);
    Q_INVOKABLE bool importDataToVault(QString sourcePath, QString vaultName, QString password);
    Q_INVOKABLE bool exportFileFromVault(QString destPath, QString vaultName, QString vaultPass, int fileId, QString fileName);

    Q_INVOKABLE QVariantList loadVaults();
    Q_INVOKABLE bool deleteVault(const QString &vaultPath);
    Q_INVOKABLE QString renameVault(const QString &oldPath, const QString &newName);

private:
    CPathDB *pathDB; // DB is used to save path of Vault Folder

private:
    bool copyRecursively(const QString &srcFilePath, const QString &targetFilePath, VaultDB &vDB, int parentId);
    bool ingestFileToBuckets(VaultDB &vDB, quint64 nodeID, QString pathSrcFile, QString pathOfVaultFolder);
    quint64 ensureAvailableBucket(VaultDB &vDB, QString pathOfVaultFolder);
    QByteArray createFileHeader(const QByteArray &masterKey, const QByteArray &contentKey);
    QByteArray decryptContentKey(const QByteArray &headerData, const QByteArray &masterKey);
};

#endif // VAULTCONTROLLER_H
