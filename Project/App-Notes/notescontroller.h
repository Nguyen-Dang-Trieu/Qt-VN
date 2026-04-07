#ifndef NOTESCONTROLLER_H
#define NOTESCONTROLLER_H

#include <QObject>

class NotesModel;

class NotesController : public QObject {
    Q_OBJECT

public:
    explicit NotesController(NotesModel *model, QObject *parent = nullptr);

    Q_INVOKABLE void addNote(QString title);
    Q_INVOKABLE void removeNote(int index);

private:
    NotesModel *m_model;
};

#endif // NOTESCONTROLLER_H
