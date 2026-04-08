#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext> // BẮT BUỘC PHẢI CÓ THÊM DÒNG NÀY
#include "vaultcontroller.h" // Include header của bạn

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // 1. Khởi tạo đối tượng Controller của bạn
    VaultController vaultController;

    QQmlApplicationEngine engine;

    // 2. KẾT NỐI C++ VỚI QML
    // Dòng này định nghĩa cái tên "vaultCtrl" để dùng trong QML
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
