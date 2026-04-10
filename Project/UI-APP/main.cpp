#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFile>
#include <QDateTime>
#include <QQmlContext>
#include "databasemanager.h"
#include "filenodemodel.h"
#include "logger.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    // Khởi tạo và kết nối DB
    auto& db = DatabaseManager::instance();
    if (db.openConnection("my_test_db.sqlite")) {
        LOG_INFO("Ket noi DB thanh cong!");

        QSqlQuery clearQuery;
            // Xóa sạch để nó chèn lại đống data 'Documents', 'Pictures' phía dưới
        clearQuery.exec("DROP TABLE IF EXISTS nodes");

        QList<QPair<QString, QString>> nodeColumns;
        nodeColumns.append(qMakePair(QString("id"), QString("INTEGER PRIMARY KEY AUTOINCREMENT")));
        nodeColumns.append(qMakePair(QString("name"), QString("TEXT NOT NULL")));
        nodeColumns.append(qMakePair(QString("type"), QString("INTEGER")));
        nodeColumns.append(qMakePair(QString("parent_id"), QString("INTEGER")));
        if(!db.createTableGeneric("Nodes", nodeColumns)) {
            LOG_ERROR("Don't Init Table Nodes");
        }

        // Tao Node Root (ID = 1)
        QSqlQuery q;
        q.exec("INSERT OR IGNORE INTO nodes (id, name, type, parent_id) VALUES (1, 'Root', 1, 0)");

        // Chen them du lieu mau de test
        q.exec("SELECT count(*) FROM nodes");
        if (q.next() && q.value(0).toInt() <= 1) {
            LOG_INFO("Inserting sample data...");

            // Cấp 1 (Con của Root - parent_id = 1)
            q.exec("INSERT INTO nodes (id, name, type, parent_id) VALUES (2, 'Documents', 1, 1)");
            q.exec("INSERT INTO nodes (id, name, type, parent_id) VALUES (3, 'Pictures', 1, 1)");
            q.exec("INSERT INTO nodes (id, name, type, parent_id) VALUES (4, 'Work_Note.docx', 2, 1)");

            // Cấp 2 (Con của Documents - parent_id = 2)
            q.exec("INSERT INTO nodes (name, type, parent_id) VALUES ('Report.pdf', 2, 2)");
            q.exec("INSERT INTO nodes (name, type, parent_id) VALUES ('Secret.txt', 2, 2)");

            // Cấp 2 (Con của Pictures - parent_id = 3)
            q.exec("INSERT INTO nodes (id, name, type, parent_id) VALUES (7, 'Vacation', 1, 3)");

            // Cấp 3 (Con của Vacation - parent_id = 7)
            q.exec("INSERT INTO nodes (name, type, parent_id) VALUES ('Sea.jpg', 2, 7)");

            LOG_INFO("Sample data inserted successfully!");
        }
    }

    // Ket noi voi database co san

    // Khoi tao model va controller
    FileNodeModel MODEL;
    MODEL.loadFolder(1);

    // VaultController CONTROLLER;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("fileNodeModel", &MODEL);
    // engine.rootContext()->setContextProperty("vaultController", &CONTROLLER);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
