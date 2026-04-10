#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QList>
#include <QPair>
#include <QDebug>
#include "logger.h"

class DatabaseManager {
public:
    // Hàm tĩnh để lấy thực thể duy nhất của class
    static DatabaseManager& instance();

    // Các hàm thao tác dữ liệu
    bool openConnection(const QString& path);
    void closeConnection();
    bool addUser(const QString& name);
    bool createTableGeneric(const QString &tableName, const QList<QPair<QString, QString>> &columns);

private:
    DatabaseManager(); // Constructor để private để không ai tự tạo được đối tượng mới
    QSqlDatabase m_db;
};

#endif // DATABASEMANAGER_H
