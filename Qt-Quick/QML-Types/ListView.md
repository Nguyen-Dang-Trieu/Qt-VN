ListView lấy dữ liệu từ:
~~~
model: notesModel
~~~
👉 Nghĩa là nó đọc dữ liệu từ notesModel (model ta cung cấp).

🧠 Nhưng thực sự chuyện gì đang xảy ra?
1. notesModel là gì?

Nó có thể là 1 trong các loại sau:

✅ Cách 1: Model viết bằng QML
~~~
ListModel {
    id: notesModel

    ListElement { title: "Note 1" }
    ListElement { title: "Note 2" }
}
~~~
👉 Khi đó:

title là role
Delegate dùng được: text: title

✅ Cách 2: Model từ C++ (chuẩn MVC bạn đang làm)

Thường bạn sẽ có:
~~~
class NotesModel : public QAbstractListModel

Trong đó:

QVariant NotesModel::data(const QModelIndex &index, int role) const
{
    if (role == TitleRole)
        return m_notes[index.row()].title;
}
~~~
👉 Và expose sang QML:

engine.rootContext()->setContextProperty("notesModel", &model);

📌 Flow đầy đủ
QML ListView
     ↓
model: notesModel
     ↓
QAbstractListModel (C++)
     ↓
data(index, role)
     ↓
trả về title
     ↓
delegate hiển thị