#ifndef FILENODEMODEL_H
#define FILENODEMODEL_H

#include <QAbstractListModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

struct NodeItem {
    int id;
    int type; // 1: Dir, 2: File
    QString name;
    qlonglong length;
};

class FileNodeModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum NodeRoles { IdRole = Qt::UserRole + 1, TypeRole, NameRole, SizeRole };

    explicit FileNodeModel(QObject *parent = nullptr) : QAbstractListModel(parent) {}

    int rowCount(const QModelIndex &parent = QModelIndex()) const override { return m_nodes.count(); }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (index.row() < 0 || index.row() >= m_nodes.count()) return QVariant();
        const NodeItem &item = m_nodes[index.row()];
        switch (role) {
            case IdRole: return item.id;
            case TypeRole: return item.type;
            case NameRole: return item.name;
            case SizeRole: return item.length;
            default: return QVariant();
        }
    }

    QHash<int, QByteArray> roleNames() const override {
        QHash<int, QByteArray> roles;
        roles[IdRole] = "nodeId";
        roles[TypeRole] = "nodeType";
        roles[NameRole] = "nodeName";
        roles[SizeRole] = "nodeSize";
        return roles;
    }

    Q_INVOKABLE void loadFolder(int parentId) {
        beginResetModel();
        m_nodes.clear();
        QSqlQuery query;
        query.prepare("SELECT id, type, name, length FROM node WHERE parent = :p");
        query.bindValue(":p", parentId);
        if (query.exec()) {
            while (query.next()) {
                m_nodes.append({query.value(0).toInt(), query.value(1).toInt(),
                                query.value(2).toString(), query.value(3).toLongLong()});
            }
        } else {
            qDebug() << "Query Error:" << query.lastError().text();
        }
        endResetModel();
    }

private:
    QList<NodeItem> m_nodes;
};

#endif
