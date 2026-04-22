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
#include "authmanager.h" // add 20261504

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
    CAuthManager authManager(vaultCtrl.getPathDB()); // add 20260415
    // nodeModel.loadFolder(1); // Load node con co parent ID = 1 (Node con cua Root)

    // Create Controller
    QObject::connect(&vaultCtrl, &VaultController::vaultOpened,
                         &nodeModel, &NodeModel::setVaultContext);

    // Chuyển qua cho QML
    engine.rootContext()->setContextProperty("vaultController", &vaultCtrl);
    engine.rootContext()->setContextProperty("listNodes", &nodeModel);
    engine.rootContext()->setContextProperty("authManager", &authManager);
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();


}
