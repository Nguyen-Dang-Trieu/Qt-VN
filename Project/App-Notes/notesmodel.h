#ifndef NOTESMODEL_H
#define NOTESMODEL_H

#include <QAbstractListModel>
#include <QList>

class NotesModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum Roles {
        // Qt::UserRole là mốc bắt đầu cho custom role, phải dùng từ đây trở đi để tránh đè lên role hệ thống
        TitleRole = Qt::UserRole + 1,
        IdRole,  // Qt::UserRole + 2
        DataRole // Qt::UserRole + 3
    };

    // User write
    explicit NotesModel(QObject *parent = nullptr);

    // Ke thua tu QAbstractListModel
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    // User write
    void insertNote(QString title); // bat buoc goi beginInsertRows(...), endInsertRows()
    void deleteNote(int index);     // bat buoc goi beginRemoveRows(...), endRemoveRows()


private:
    // Form of data
    struct Note {
        int id;
        QString title;
    };

    QList<Note> m_notes; // Noi chua du lieu
};

#endif // NOTESMODEL_H
