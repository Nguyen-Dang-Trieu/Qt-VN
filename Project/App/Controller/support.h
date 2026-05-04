#ifndef SUPPORT_H
#define SUPPORT_H

#include <QByteArray>
#include <openssl/rand.h>
#include <QRandomGenerator>
#include <QVariant>

// New add: 2026-04-29 [Drew]
struct ChunkData {
    quint64    slice_number;
    QString    sliceUuid;
    QByteArray plainData;
    QByteArray encryptedData;
    QByteArray nonce;
    QByteArray tag;
};

// New add: 2026-04-29 [Drew]
struct FileProcessTask {
    quint64 nodeId;
    QString sourcePath;
    qint64  fileSize;
};

QByteArray randomKey(void);
QByteArray randomHeaderNonce(void);
// Random char to add slice_uuid
QString generateRandomLetters(int len = 6);
QString getDbPathFromVaultFolder(const QString &vaultPath);

#endif // SUPPORT_H
