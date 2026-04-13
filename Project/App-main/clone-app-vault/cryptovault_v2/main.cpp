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

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    // ---- Test

    //----
    QQmlApplicationEngine engine;

    // Create Controller
    VaultController vaultCtrl; // CConstructor VaultController::VaultController() duoc call
    engine.rootContext()->setContextProperty("vaultController", &vaultCtrl);

    NodeModel nodeModel;
    nodeModel.loadFolder(1); // Load node con co parent ID = 1 (Node con cua Root)
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
