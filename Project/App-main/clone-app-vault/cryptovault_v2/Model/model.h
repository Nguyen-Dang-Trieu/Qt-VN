#ifndef MODEL_H
#define MODEL_H


#include <QAbstractListModel>
#include "Tbl_Node.h"
#include "ezsql.h"
#include "VaultDB.h"
#include "logger.h"

class NodeModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum NodeRoles {
        IdRole = Qt::UserRole + 1,
        TypeRole,
        NameRole };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        Q_UNUSED(parent);
        return Nodes.count();
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (index.row() < 0 || index.row() >= Nodes.count()) return QVariant();
        const Node &item = Nodes[index.row()];

        switch (role) {
        case IdRole: return item.m_id;
        case TypeRole: return item.m_type;
        case NameRole: return item.m_name;
        default: return QVariant();
        }
    }

    QHash<int, QByteArray> roleNames() const override {
        QHash<int, QByteArray> roles;
        roles[IdRole] = "nodeId";
        roles[TypeRole] = "nodeType";
        roles[NameRole] = "nodeName";
        return roles;
    }

    Q_INVOKABLE void loadFolder(int parentId) {
        beginResetModel();

        Nodes.clear();

        VaultDB vDB;
        QString dbPath     = "D:\\Qt\\Work\\App\\clone-app-vault\\clone-app-vault\\output_test_app\\hello\\hello.db";
        QString passwordDB = "emptyKey";
        bool isDbReady = vDB.open(dbPath, passwordDB);
        if(!isDbReady) {
            LOG_ERROR("Model can't open hello.DB");
        }

        vDB.node.setDb(&vDB._db_);

        // Tien hanh lay du lieu node (file/folder) tu trong table node in Vault DB
        QString query = QString("WHERE parent_id = %1").arg(parentId); // '%1' -> %1 boi vi parent ID la INTERGER
        QVector<Node> results = vDB.node.fetch(query);
        if(!results.empty()) {
            LOG_ERROR("Model can't get data node from Table node in DB!");
        }

        for (const Node &curNode : results) {
            Node item;
            item.m_id     = curNode.m_id;
            item.m_name   = curNode.m_name;
            item.m_type   = curNode.m_type;
            item.m_parent = curNode.m_parent;

            Nodes.append(item);
        }

        endResetModel();
    }

public slots:
    void setVaultContext(const QString &dbPath, const QString &password) {
        m_currentDbPath = dbPath;
        m_currentPassword = password;

        // Tự động load thư mục gốc (parentId = 1) khi được set context mới
        loadFolder(1);
    }

private:
    QVector<Node> Nodes;         // Nơi chứa dữ liệu phù hợp theo parrent ID do VIEW truyền vào
    QString m_currentDbPath;     // Lưu đường dẫn
    QString m_currentPassword;   // Lưu mật khẩu
};
#endif // MODEL_H
