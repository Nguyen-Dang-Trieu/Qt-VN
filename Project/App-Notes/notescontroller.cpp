#include "notescontroller.h"
#include "notesmodel.h"

NotesController::NotesController(NotesModel *model, QObject *parent)
    : QObject(parent), m_model(model) {}

void NotesController::addNote(QString title) {
    if (title.trimmed().isEmpty())
        return;

    m_model->insertNote(title.trimmed());
}

void NotesController::removeNote(int index) {
    m_model->deleteNote(index);
}
