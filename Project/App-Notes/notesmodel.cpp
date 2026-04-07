#include "notesmodel.h"
#include <QSqlQuery>
#include <QVariant>

NotesModel::NotesModel(QObject *parent)
    : QAbstractListModel(parent)
{
    QSqlQuery query("SELECT id, title FROM notes");

    while (query.next()) {
        m_notes.append({
            query.value(0).toInt(),
            query.value(1).toString()
        });
    }
}

int NotesModel::rowCount(const QModelIndex &parent) const {
    if(parent.isValid())
        return 0;
    return m_notes.size();
}

/*
QModeIndex đại diện cho vị trí Item (row, column)

*/
QVariant NotesModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) return {};

    // Lấy dữ liệu từ container "QList<Note> m_notes"
    const Note &note = m_notes[index.row()];

    // Tách lấy dữ liệu mà UI yêu cầu
    if (role == TitleRole)
        return note.title;

    return {};
}

QHash<int, QByteArray> NotesModel::roleNames() const {
    return {
        {TitleRole, "title"}
    };
}

void NotesModel::insertNote(QString title) {
    QSqlQuery query;
    query.prepare("INSERT INTO notes (title) VALUES (?)");
    query.addBindValue(title);
    query.exec();

    int id = query.lastInsertId().toInt();

    beginInsertRows(QModelIndex(), m_notes.size(), m_notes.size());
    m_notes.append({id, title});
    endInsertRows();
}

void NotesModel::deleteNote(int index) {
    if (index < 0 || index >= m_notes.size()) return;

    int id = m_notes[index].id;

    QSqlQuery query;
    query.prepare("DELETE FROM notes WHERE id=?");
    query.addBindValue(id);
    query.exec();

    beginRemoveRows(QModelIndex(), index, index);
    m_notes.removeAt(index);
    endRemoveRows();
}
