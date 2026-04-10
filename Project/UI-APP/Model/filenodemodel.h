#ifndef FILENODEMODEL_H
#define FILENODEMODEL_H

// Đọc dữ liệu từ DB và đưa lên cho View

#include <QAbstractListModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>


struct NodeItem {
    int id;
    int type;
    QString name;
    int parent_id;
};

class FileNodeModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum NodeRoles {
        IdRole = Qt::UserRole + 1,
        TypeRole,
        NameRole,
        ParrentIDRole
    };
    explicit FileNodeModel(QObject *parent = nullptr) : QAbstractListModel(parent) {}

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        return storageAllNodes.count();
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (index.row() < 0 || index.row() >= storageAllNodes.count()) return QVariant();

        const NodeItem &item = storageAllNodes[index.row()];
        switch (role) {
        case IdRole: return item.id;
        case TypeRole: return item.type;
        case NameRole: return item.name;
        case ParrentIDRole: return item.parent_id;
        default: return QVariant();
        }
    }

    QHash<int, QByteArray> roleNames() const override {
        QHash<int, QByteArray> roles;
        roles[IdRole]        = "nodeId";  // Ví dụ khi QML gọi "nodeId", hãy đưa "IdRole"
        roles[TypeRole]      = "nodeType";
        roles[NameRole]      = "nodeName";
        roles[ParrentIDRole] = "nodeParrentID";
        return roles;
    }

    // View sẽ gọi hàm này khi muốn xem nội dung của một thư mục
    Q_INVOKABLE void loadFolder(int parentId) {
        beginResetModel();

        storageAllNodes.clear(); // làm sạch kho lưu trữ

        // Bắt đầu quá trình tìm kiếm
        QSqlQuery query;
        query.prepare("SELECT id, name, type, parent_id FROM nodes WHERE parent_id = :parent");
        query.bindValue(":parent", parentId);

        if (query.exec()) {
            while (query.next()) {
                NodeItem item;
                item.id = query.value("id").toInt();
                item.name = query.value("name").toString();
                item.type = query.value("type").toInt();
                item.parent_id = query.value("parent_id").toInt();

                storageAllNodes.append(item); // Khi tìm thấy một phần tử phù hợp sẽ được thêm vào stora
            }
        } else {
            qDebug() << "Load Folder Error:" << query.lastError().text();
        }
        endResetModel();
    }

    //    void setDatabase(EzSql::DataBase* db) { m_db = db; }
private:

    QVector<NodeItem> storageAllNodes; // Danh sách lưu trữ tát cả các node item khi hiển thị lên VIEW
    // View sẽ  lấy dữ liệu từ trong storageAllNodes
};
#endif // FILENODEMODEL_H
