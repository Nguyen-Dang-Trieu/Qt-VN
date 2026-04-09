#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "filenodemodel.h"

void setupDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:"); // Dùng RAM cho nhanh khi test
    if (!db.open()) return;

    QSqlQuery q;
    q.exec("CREATE TABLE node (id INTEGER PRIMARY KEY, type INTEGER, name TEXT, parent INTEGER, length INTEGER)");

    // Dữ liệu mẫu: Cấu trúc Root (1) -> Documents (2) -> Project (4)
    q.exec("INSERT INTO node VALUES (1, 1, 'Root', 0, 0)");
    q.exec("INSERT INTO node VALUES (2, 1, 'Documents', 1, 0)");
    q.exec("INSERT INTO node VALUES (3, 2, 'secret_key.txt', 1, 1024)");
    q.exec("INSERT INTO node VALUES (4, 1, 'Project_A', 2, 0)");
    q.exec("INSERT INTO node VALUES (5, 2, 'budget.xlsx', 2, 50000)");
    q.exec("INSERT INTO node VALUES (6, 2, 'main.cpp', 4, 2500)");
}

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    setupDatabase();


    FileNodeModel model;
    model.loadFolder(1); // Load Root mặc định

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("fileNodeModel", &model);
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
