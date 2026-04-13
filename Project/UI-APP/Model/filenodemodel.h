#ifndef FILENODEMODEL_H
#define FILENODEMODEL_H

// Đọc dữ liệu từ DB và đưa lên cho View

#include <QAbstractListModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>


// Đại diện cho một node item, có thể là file hoặc folder
struct NodeItem {
    int id;
    int type;
    QString name;
    int parent_id;
};

class FileNodeModel : public QAbstractListModel {
    Q_OBJECT
public:

    // Định nghĩa các role để QML có thể truy cập dữ liệu một cách dễ dàng
    enum NodeRoles {
        IdRole = Qt::UserRole + 1, // Bắt đầu từ Qt::UserRole để tránh xung đột với các role mặc định của Qt
        TypeRole,
        NameRole,
        ParrentIDRole
    };
    explicit FileNodeModel(QObject *parent = nullptr) : QAbstractListModel(parent) {}

    /**
    Ý nghĩa: Khi giao diện (View) muốn vẽ danh sách, câu hỏi đầu tiên nó hỏi Model là: "Danh sách này 
    có bao nhiêu phần tử để còn biết đường tạo ra bấy nhiêu ô?".

    Hoạt động: Hàm này trả về số lượng phần tử hiện đang có trong mảng storageAllNodes thông qua 
    storageAllNodes.count().
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        Q_UNUSED(parent);
        return storageAllNodes.count();
    }

    /**
     * Ý nghĩa: Sau khi biết số lượng qua rowCount(), View sẽ duyệt qua từng phần tử và hỏi Model: 
     * "Ở dòng số X (index), thuộc tính Y (role) có giá trị là gì?".
     */

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (index.row() < 0 || index.row() >= storageAllNodes.count()) return QVariant();

        const NodeItem &item = storageAllNodes[index.row()];

        // Dựa vào role mà View yêu cầu, trả về giá trị tương ứng từ NodeItem
        switch (role) {
        case IdRole: return item.id;
        case TypeRole: return item.type;
        case NameRole: return item.name;
        case ParrentIDRole: return item.parent_id;
        default: return QVariant();
        }
    }

    // Mapping giữa role và tên mà QML sẽ sử dụng để truy cập dữ liệu
    QHash<int, QByteArray> roleNames() const override {
        QHash<int, QByteArray> roles;
        roles[IdRole]        = "nodeId";  // Ví dụ khi QML gọi "nodeId", hãy đưa "IdRole"
        roles[TypeRole]      = "nodeType";
        roles[NameRole]      = "nodeName";
        roles[ParrentIDRole] = "nodeParrentID";
        return roles;
    }

    /**
     * View sẽ gọi hàm này khi muốn xem nội dung của một thư mục
     * 
     * Q_INVOKABLE cho phép hàm này có thể được gọi trực tiếp từ QML, giúp giao diện có thể yêu cầu 
     * Model tải dữ liệu mới khi người dùng tương tác (ví dụ: nhấp vào một thư mục để xem nội dung bên 
     * trong).
     * 
     */
    // Tham số parentId: View truyền vào
    Q_INVOKABLE void loadFolder(int parentId) {
        /**
         * Phát tín hiệu báo cho View biết: "Sắp có thay đổi dữ liệu lớn, hãy ngừng vẽ lại đi".
         */
        beginResetModel();

        // Xóa hết dữ liệu cũ trong storageAllNodes để chuẩn bị cho dữ liệu mới
        storageAllNodes.clear(); 

        /* Bắt đầu quá trình tìm kiếm */ 
        QSqlQuery query;

        // Tìm tất cả các node (file/folder) có parent_id bằng với "parentId" mà View yêu cầu
        query.prepare("SELECT id, name, type, parent_id FROM nodes WHERE parent_id = :parent");
        query.bindValue(":parent", parentId);

        if (query.exec()) {
            while (query.next()) {
                // Đọc từng dòng kết quả từ table in DB, và thêm vào biến item
                NodeItem item;
                item.id = query.value("id").toInt();
                item.name = query.value("name").toString();
                item.type = query.value("type").toInt();
                item.parent_id = query.value("parent_id").toInt();

                // Them item vào storageAllNodes để lưu trữ, sau đó View sẽ lấy dữ liệu từ đây để vẽ
                storageAllNodes.append(item); 
            }
        } else {
            qDebug() << "Load Folder Error:" << query.lastError().text();
        }
        /**
         * Phát tín hiệu báo cho View biết: "Cập nhật dữ liệu xong rồi, anh load lại danh sách mới đi". 
         * Lúc này, View sẽ tự động gọi lại hàm rowCount() và data() để vẽ lại giao diện.
         */
        endResetModel();
    }

    //    void setDatabase(EzSql::DataBase* db) { m_db = db; }
private:

    QVector<NodeItem> storageAllNodes; // Danh sách lưu trữ tát cả các node item khi hiển thị lên VIEW
    // View sẽ  lấy dữ liệu từ trong storageAllNodes
};
#endif // FILENODEMODEL_H
