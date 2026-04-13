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

// Ops with cloumn "slices" of Table bucket
QJsonArray parseSlices(const QByteArray &raw)
{
    if (raw.isEmpty()) return QJsonArray();

    QJsonDocument doc = QJsonDocument::fromJson(raw);
    if (!doc.isArray()) return QJsonArray();

    return doc.array();
}

void name(void) {
    QJsonObject obj;
}

// Convert JSON -> DB
QByteArray stringifySlices(const QJsonArray &arr)
{
    QJsonDocument doc(arr);
    return doc.toJson(QJsonDocument::Compact);
}

bool addSliceToBucket(Bucket &bucket, quint64 offset, const QString &uuid)
{
    QJsonArray arr = parseSlices(bucket.slices);

    QJsonObject obj;
    obj["slice_uuid"] = uuid;
    obj["offset"] = QString::number(offset);

    arr.append(obj);

    bucket.slices = stringifySlices(arr);
    return true;
}

//QJsonArray getSortedSlices(const Bucket &bucket)
//{
//    QJsonArray arr = parseSlices(bucket.slices);

//    std::sort(arr.begin(), arr.end(), [](const QJsonValue &a, const QJsonValue &b) {
//        quint64 oa = a.toObject()["offset"].toString().toULongLong();
//        quint64 ob = b.toObject()["offset"].toString().toULongLong();
//        return oa < ob;
//    });

//    return arr;
//}

bool findOffsetByUuid(const Bucket &bucket, const QString &uuid, quint64 &offsetOut)
{
    QJsonArray arr = parseSlices(bucket.slices);

    for (const auto &v : arr) {
        QJsonObject obj = v.toObject();
        if (obj["slice_uuid"].toString() == uuid) {
            offsetOut = obj["offset"].toString().toULongLong();
            return true;
        }
    }
    return false;
}

quint64 getNextOffset(const QString &bucketPath)
{
    QFile file(bucketPath);
    if (!file.open(QIODevice::ReadOnly)) return 0;

    quint64 size = file.size();
    file.close();
    return size;
}

QByteArray readSlice(const Bucket &bucket, const QString &bucketPath, const QString &uuid)
{
    quint64 offset = 0;
    if (!findOffsetByUuid(bucket, uuid, offset)) return {};

    QFile file(bucketPath);
    if (!file.open(QIODevice::ReadOnly)) return {};

    file.seek(offset);
    QByteArray data = file.read(SLICE_SIZE);

    file.close();
    return data;
}


//
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

