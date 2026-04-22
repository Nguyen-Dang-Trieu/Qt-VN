#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFile>
#include <QDateTime>
#include <QQmlContext>

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/bio.h>

// Algorithm
#include "aes_256_gcm.h"

// Model
#include "vaultcontroller.h"
#include "filenodemodel.h"
// Debug
#include "logger.h"

static const unsigned char gcm_key[] = { // 32 bytes
    0xee, 0xbc, 0x1f, 0x57, 0x48, 0x7f, 0x51, 0x92, 0x1c, 0x04, 0x65, 0x66,
    0x5f, 0x8a, 0xe6, 0xd1, 0x65, 0x8b, 0xb2, 0x6d, 0xe6, 0xf8, 0xa0, 0x69,
    0xa3, 0x52, 0x02, 0x93, 0xa5, 0x72, 0x07, 0x8f
};

// GCM standard khuyến nghị: IV = 12 bytes (96-bit)
static const unsigned char gcm_iv[] = { // 16 bytes
    0x99, 0xaa, 0x3e, 0x68, 0xed, 0x81, 0x73, 0xa0, 0xee, 0xd0, 0x66, 0x84
};

static const unsigned char gcm_pt[] = { // plain-text
    0xf5, 0x6e, 0x87, 0x05, 0x5b, 0xc3, 0x2d, 0x0e, 0xeb, 0x31, 0xb2, 0xea,
    0xcc, 0x2b, 0xf2, 0xa5
};

static const unsigned char gcm_aad[] = {
    0x4d, 0x23, 0xc3, 0xce, 0xc3, 0x34, 0xb4, 0x9b, 0xdb, 0x37, 0x0c, 0x43,
    0x7f, 0xec, 0x78, 0xde
};

void runAlgorithmTest() {
    qDebug() << "---------- RUNNING OPENSSL 1.1.1 TEST ----------";

    // Chuyển dữ liệu static sang QByteArray
    QByteArray key = QByteArray::fromRawData(reinterpret_cast<const char*>(gcm_key), sizeof(gcm_key));
    QByteArray iv  = QByteArray::fromRawData(reinterpret_cast<const char*>(gcm_iv), sizeof(gcm_iv));
    QByteArray pt  = QByteArray::fromRawData(reinterpret_cast<const char*>(gcm_pt), sizeof(gcm_pt));
    QByteArray aad = QByteArray::fromRawData(reinterpret_cast<const char*>(gcm_aad), sizeof(gcm_aad));

    QByteArray ciphertext;
    QByteArray tag;

    // 1. Test Encrypt
    if (aes_gcm_encrypt(pt, key, iv, aad, ciphertext, tag)) {
        qDebug() << "Encrypt OK!";
        qDebug() << "Ciphertext (hex):" << ciphertext.toHex().toUpper();
        qDebug() << "Tag (hex):"        << tag.toHex().toUpper();

        // So sánh với kết quả mong đợi của NIST nếu cần
        // Expected CT: F7264413A84C0E7CD536867EB9F21736
    } else {
        qCritical() << "Encrypt Failed!";
        return;
    }

    // 2. Test Decrypt
    QByteArray decrypted = aes_gcm_decrypt(ciphertext, tag, key, iv, aad);
    if (!decrypted.isEmpty()) {
        qDebug() << "Decrypt OK!";
        qDebug() << "Decrypted (hex):" << decrypted.toHex().toUpper();

        if (decrypted == pt) {
            qDebug() << "MATCH: Decrypted data is identical to plaintext!";
        } else {
            qWarning() << "MISMATCH: Decrypted data differs from plaintext!";
        }
    } else {
        qCritical() << "Decrypt Failed (Tag Authentication Failed)!";
    }

    qDebug() << "-----------------------------------------------";
}

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

   runAlgorithmTest();


    QQmlApplicationEngine engine;
    // Create Controller
    VaultController vaultCtrl;
    engine.rootContext()->setContextProperty("vaultController", &vaultCtrl);

    FileNodeModel fileNodeModel;
    engine.rootContext()->setContextProperty("fileNodeModel", &fileNodeModel);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
