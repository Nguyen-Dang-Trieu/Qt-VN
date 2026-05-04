#ifndef VAULTCONTROLLER_H
#define VAULTCONTROLLER_H

#include <QObject>
#include <QUrl>
#include <QDir>
#include <QDebug>
#include <QFileInfo>
#include <QtConcurrent> // NEW 2026-04-20
#include <QJsonObject>

#include "VaultDB.h"
#include "PathDB.h"
#include "support.h"

#define BUCKET_SIZE         (128 * 1024 * 1024)                   // 128 MiB
#define CHUNK_SIZE          (32 * 1024)                           // 32 KiB
#define SLICE_SIZE          (CHUNK_SIZE + 12 + 16)                // (ciphertext + IV + Tag
#define SLICES_PER_BUCKET   (BUCKET_SIZE / SLICE_SIZE)

// Scale
//#define BUCKET_SIZE       (1024 * 1024)                 // 1 MiB  - 1.048.576 Bytes
//#define CHUNK_SIZE        (32 * 1024)                   // 32 KiB - 32 768 Bytes
//#define SLICE_SIZE        (CHUNK_SIZE + 28)             // CHUNK_SIZE + 12 IV + 16 Tag
//#define SLICES_PER_BUCKET (BUCKET_SIZE / SLICE_SIZE)    // 31 slices mỗi bucket

// NEW 2026-04-15
class VaultController : public QObject {
    Q_OBJECT

signals:
    // Signal phát ra khi Vault được mở thành công (hoặc khi cần truyền data cho Model)
    void vaultOpened(const QString &vaultName, const QString &passVault, const QString &pathDB); // NEW
    void importFinished(bool success);

public:
    explicit VaultController(QObject *parent = nullptr);

    Q_INVOKABLE bool openVault(const QString &vaultName, const QString &password, const QString &path); // NEW

    Q_INVOKABLE bool createVaultFolder(const QString &vaultName, const QString &password,  const QString &path);
    Q_INVOKABLE bool importDataToVaultAsync(const QStringList &sourcePaths, const QString &vaultName, const QString &password);
    // Q_INVOKABLE bool importDataToVault(const QString sourcePath, const QString vaultName, const QString password);
    Q_INVOKABLE bool exportFileFromVault(QString destPath, QString vaultName, QString vaultPass, int fileId, QString fileName, int fileType);

    Q_INVOKABLE QVariantList loadVaults();
    Q_INVOKABLE bool deleteVault(const QString &vaultPath);
    Q_INVOKABLE QString renameVault(const QString &oldPath, const QString &newNameVault);

    Q_INVOKABLE bool deleteFile(const QString &nameVault, const QString &passVault, const QString &fileName, const QString &fileID);

    Q_INVOKABLE bool checkVaultStructure(const QString &folderPath);
    CPathDB* getPathDB() const { return pathDB; }
    Q_INVOKABLE bool addExistingVault(const QString &name, const QString &path) {
        return pathDB->saveVaultFolder(name, "", path);
    }

    // add start 20260417
    Q_INVOKABLE QString changeVaultPassword(const QString &vaultName, const QString &vaultPath, const QString &currentPass, const QString &newPass);
    // add end 20260417

    // aadd start 20260420
    Q_INVOKABLE bool verifyVaultKEK(const QString &vaultName, const QString &vaultPath, const QString &kekHex);
    Q_INVOKABLE QJsonObject resetVaultPasswordWithKEK(const QString &vaultName, const QString &vaultPath, const QString &kekHex, const QString &newPass);
    // add end 20260420

private:
    CPathDB *pathDB; // DB is used to save path of Vault Folder

private:

    // Heler functions
    bool scanAndInsertNodes(const QString &srcFilePath, VaultDB &vDB, int parentId, QList<FileProcessTask> &pendingFiles);
    bool ingestFileToBuckets(VaultDB &vDB,
                             quint64 nodeID,
                             QString pathSrcFile,
                             QString pathOfVaultFolder,
                             quint64 &cachedBucketID,
                             qint64 fileSize);
    quint64 ensureAvailableBucket(VaultDB &vDB, QString pathOfVaultFolder);

    bool exportNodeRecursive(VaultDB &vDB, QString destPath, int nodeId, QString nodeName, int nodeType, const QByteArray &contentKey, const QString &vaultPath);
    bool decryptFile(VaultDB &vDB, QString destPath, int fileId, QString fileName, const QByteArray &contentKey, const QString &vaultPath);

    // Ops for Array Slices
    qint64 findAvailableOffset(const QJsonArray &arrSlices);
};

#endif // VAULTCONTROLLER_H

