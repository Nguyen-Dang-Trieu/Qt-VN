#include "support.h"
#include "logger.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QIODevice>

#include "Tbl_Bucket.h"
#include "vaultcontroller.h"

QByteArray randomKey(void) {
    QByteArray Key(32, 0); // 32 Bytes
    if (!RAND_bytes(reinterpret_cast<unsigned char*>(Key.data()), Key.size())) {
        LOG_ERROR("Random Key to failed");
    }

    return Key;
}

// HeaderNonce (IV)
QByteArray randomHeaderNonce(void) {
    QByteArray headerNonce(12, 0); // 12 Bytes
    if (!RAND_bytes(reinterpret_cast<unsigned char*>(headerNonce.data()), headerNonce.size())) {
        LOG_ERROR("Random Key to failed");
    }

    return headerNonce;
}


QString generateRandomLetters(int len)
{
    const QString chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    QString out;

    for (int i = 0; i < len; ++i) {
        int idx = QRandomGenerator::global()->bounded(chars.size());
        out += chars[idx];
    }

    return out;
}

