#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "vaultcontroller.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // 1. Khởi tạo đối tượng Controller của bạn
    VaultController vaultController;

    QQmlApplicationEngine engine;

    // 2. KẾT NỐI C++ VỚI QML
    engine.rootContext()->setContextProperty("vaultCtrl", &vaultController);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}