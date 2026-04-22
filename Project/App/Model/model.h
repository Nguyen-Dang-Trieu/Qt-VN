#ifndef MODEL_H
#define MODEL_H

#include <QStandardPaths>
#include <QAbstractListModel>
#include "Tbl_Node.h"
#include "ezsql.h"
#include "VaultDB.h"
#include "logger.h"
#include "PathDB.h"
#include "support.h"

class NodeModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum NodeRoles {
        IdRole = Qt::UserRole + 1,
        TypeRole,
        NameRole,
        // add start 20260414
        ATimeRole,
        MTimeRole,
        LengthRole
        // add end 20260414
    };

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
            // add start 20260414
        case ATimeRole: return QVariant::fromValue(item.m_atime);
        case MTimeRole: return QVariant::fromValue(item.m_time);
        case LengthRole: return QVariant::fromValue(item.m_length);
            // add end 20260414
        default: return QVariant();
        }
    }

    QHash<int, QByteArray> roleNames() const override {
        QHash<int, QByteArray> roles;
        roles[IdRole] = "nodeId";
        roles[TypeRole] = "nodeType";
        roles[NameRole] = "nodeName";
        // add start 20260414
        roles[ATimeRole] = "nodeAtime";
        roles[MTimeRole] = "nodeMtime";
        roles[LengthRole] = "nodeLength";
        // add end 20260414
        return roles;
    }

    // NEW 2026-04-20
    Q_INVOKABLE void loadFolder(int parentId) {
        beginResetModel();
        /* LOG DEBUG */
        LOG_DEBUG("[MODEL] Name Vault: " + m_currentVautName);    // Ex: "hello"
        LOG_DEBUG("[MODEL] Pass Vault: " + m_currentPassword);    // Ex: "123456"
        LOG_DEBUG("[MODEL] Path Vault: " + m_currentPathVault);   // Ex: "/home/GhostRider/Work/clone-app-vault_/output_test_app/hello"

        Nodes.clear(); // Xoa toan bo du lieu hien tai dang chua trong QVector<Node> Nodes

        VaultDB vDB;
        QString fullPathDB = getDbPathFromVaultFolder(m_currentPathVault);

        LOG_DEBUG("[MODEL] " + fullPathDB);

        QString passwordDB = m_currentPassword;
        bool    isDbReady  = vDB.open(fullPathDB, passwordDB);

        if(!isDbReady) {
            LOG_ERROR("Model can't open hello.DB");
        }

        LOG_WARNING("[MODEL] Login " + m_currentVautName + " DB Successfull!");
        vDB.node.setDb(&vDB._db_);

        // Tien hanh lay du lieu node (file/folder) tu trong table node in Vault DB
        QString query = QString("WHERE parent_id = %1").arg(parentId); // '%1' -> %1 boi vi parent ID la INTERGER
        QVector<Node> results = vDB.node.fetch(query);
        if(results.empty()) {
            LOG_ERROR("Model can't get data node from Table node in DB!");
        }

        for (const Node &curNode : results) {
            Node item;
            item.m_id     = curNode.m_id;
            item.m_name   = curNode.m_name;
            item.m_type   = curNode.m_type;
            item.m_parent = curNode.m_parent;
            // add start 20260414
            item.m_atime  = curNode.m_atime;
            item.m_time   = curNode.m_time;
            item.m_length = curNode.m_length;
            // add end 20260414
            Nodes.append(item);
        }

        endResetModel();
    }

public slots:
    void setVaultContext(const QString &vaultName, const QString &passVault, const QString &pathDB) {
        //        LOG_DEBUG("[MODEL] Name DB: " + vaultName);
        //        LOG_DEBUG("[MODEL] Pass DB: " + passVault);
        //        LOG_DEBUG("[MODEL] Path DB: " + pathDB);

        m_currentVautName = vaultName;
        m_currentPassword = passVault;
        m_currentPathVault   = pathDB;

        // Tự động load thư mục gốc (parentId = 1) khi được set context mới
        loadFolder(1);
    }

    // add start 20260415
    Q_INVOKABLE bool checkPassword(const QString &vaultName, const QString &passVault, const QString &pathDB);
    // add end 20260415

    // add start 20260417
    Q_INVOKABLE QString getVaultKEK(const QString &vaultName, const QString &passVault, const QString &pathDB);
    // add end 20260417

    // add start 20260420
    Q_INVOKABLE QString getAccountKEK(const QString &username, const QString &password);
    // add end 20260420

private:
    QVector<Node> Nodes;         // Nơi chứa dữ liệu phù hợp parrent ID do VIEW truyền vào

    QString m_currentVautName;   // Lưu tên vault
    QString m_currentPassword;   // Lưu mật khẩu vault = mật khẩu DB vault
    QString m_currentPathVault;     // Lưu đường dẫn DB vault

};
#endif // MODEL_H
