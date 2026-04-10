#include "databasemanager.h"
#include <QDebug>

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager instance;
    return instance;
}

DatabaseManager::DatabaseManager() {
    // Khởi tạo driver SQLite
    m_db = QSqlDatabase::addDatabase("QSQLITE");
}

bool DatabaseManager::openConnection(const QString& path) {
    m_db.setDatabaseName(path);
    if (!m_db.open()) {
        qDebug() << "Lỗi kết nối:" << m_db.lastError().text();
        return false;
    }
    return true;
}

void DatabaseManager::closeConnection() {
    m_db.close();
}

bool DatabaseManager::createTableGeneric(const QString &tableName, const QList<QPair<QString, QString>> &columns) {
    if (tableName.isEmpty() || columns.isEmpty()) return false;

    QString sql = QString("CREATE TABLE IF NOT EXISTS %1 (").arg(tableName);

    QStringList columnDefinitions;

    // Duyệt qua QList thay vì QMap
    for (const auto &column : columns) {
        // column.first là tên cột, column.second là kiểu dữ liệu
        columnDefinitions << QString("%1 %2").arg(column.first, column.second);
    }

    sql += columnDefinitions.join(", ");
    sql += ")";

    QSqlQuery query;
    if (!query.exec(sql)) {
        qDebug() << "Lỗi SQL:" << query.lastError().text();
        qDebug() << "Câu lệnh lỗi:" << sql;
        return false;
    }

    return true;
}

bool DatabaseManager::addUser(const QString& name) {
    QSqlQuery query;
    query.prepare("INSERT INTO users (name) VALUES (:name)");
    query.bindValue(":name", name);
    return query.exec();
}
