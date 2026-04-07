#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSqlDatabase>
#include <QSqlQuery>


#include "notescontroller.h"
#include "notesmodel.h"


void initDB() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("notes.db");

    if (!db.open()) {
        qDebug() << "DB open failed";
        return;
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS notes ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "title TEXT)");
}

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    initDB();

    NotesModel model;
    NotesController controller(&model);


    // Hien thi model va controller trong qml
    QQmlApplicationEngine engine; 
    engine.rootContext()->setContextProperty("notesModel", &model);
    engine.rootContext()->setContextProperty("notesController", &controller);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
