#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFile>
#include <QDateTime>
#include <QQmlContext>

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/bio.h>


#include "vaultcontroller.h" // Controller
#include "model.h"   // Model


void createTestFiles(const QString &folder)
{
    QDir().mkpath(folder);

    // Bắt đầu từ 64 KiB
    qint64 currentSize = 64 * 1024;

    for (int i = 0; i < 10; i++)
    {
        // Xác định ký tự cho file (0 -> 'A', 1 -> 'B', ...)
        char ch = static_cast<char>('A' + i);

        // Tên file theo định dạng: file_1_A.txt, file_2_B.txt...
        QString fileName = folder + QString("/file_%1_%2.txt").arg(i + 1).arg(ch);
        QFile file(fileName);

        if (!file.open(QIODevice::WriteOnly)) {
            qDebug() << "Không thể tạo file:" << fileName;
            continue;
        }

        // Tạo buffer 64KiB chứa toàn bộ ký tự ch
        QByteArray buffer(64 * 1024, ch);
        qint64 bytesToWrite = currentSize;

        // Ghi cho đến khi đủ dung lượng file
        while (bytesToWrite > 0) {
            qint64 chunk = qMin((qint64)buffer.size(), bytesToWrite);
            file.write(buffer.data(), chunk);
            bytesToWrite -= chunk;
        }

        file.close();

        qDebug() << QString("Đã tạo: %1 | Dung lượng: %2 KiB | Ký tự: '%3'")
                    .arg(fileName)
                    .arg(currentSize / 1024)
                    .arg(ch);

        // Gấp đôi kích thước cho file tiếp theo
        currentSize *= 2;
    }

    qDebug() << "--- Hoàn thành tạo 10 file test ---";
}

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    // ---- Test
//    createTestFiles("/home/GhostRider/Work/clone-app-vault_/cryptovault_v2/Test/Data_test/");
    //----
    QQmlApplicationEngine engine;

    // Khởi tạo controller và model
    VaultController vaultCtrl; // CConstructor VaultController::VaultController() duoc call
    NodeModel       nodeModel;
    // nodeModel.loadFolder(1); // Load node con co parent ID = 1 (Node con cua Root)

    // Create Controller
    QObject::connect(&vaultCtrl, &VaultController::vaultOpened,
                         &nodeModel, &NodeModel::setVaultContext, Qt::QueuedConnection);

    // Chuyển qua cho QML
    engine.rootContext()->setContextProperty("vaultController", &vaultCtrl);
    engine.rootContext()->setContextProperty("listNodes", &nodeModel);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
