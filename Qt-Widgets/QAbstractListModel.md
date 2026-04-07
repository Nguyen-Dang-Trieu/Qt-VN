doc: https://doc.qt.io/qt-6/qabstractlistmodel.html

De de hinh dung hon chi nghi ve data

~~~
QList<QString> notes = {
    "Hoc Qt",
    "Di cho",
    "Code embedded"
};
~~~
-> Day la model o muc co ban nhat

Co 2 cau hoi:
- Co bao nhieu notes
- notes[i] la gi

Trien khai thong qua C++
~~~
int count = notes.size();
QString note = notes[i];
~~~

Buoc2: Bay gio UI can hien thi mot list cac notes. UI can biet
- Co bao nhieu item
- Moi item la gi
- Khi data thay doi thi Item se ra sao

=> Van de QML khong the truy xuat data tu QList -> QAbstractListModel duoc tao ra

Buoc3: QAbstractListModel yeu cau viet lai 3 ham nay
- rowCount() -> co bao nhieu item
- QVariant QVariant data(index, role) -> data co hinh dang nhu the nao
- roleNames() -> Dat ten data ma QML  su dung


Buoc4: Mapping sang code
- Data that: QList<Note> m_notes;
- rowCount() -> return m_notes.size()
- data() -> return m_notes[index.row()].title
- roleNames() -> "title"

Buoc5: QML su dung
~~~
ListView {
    model: notesModel

    delegate: Text {
        text: title
    }
}
~~~

QML se thuc hien
for i = 0 -> rowCount():
    goi data(i)
    hien thi title

Buoc6: Update data
Gia su bay gio them notes moi
~~~
m_notes.append("New note");
~~~

UI khong biet

-> Can dung
~~~
beginInsertRows()
endInsertRows()
~~~

Y nghia:
~~~
beginInsertRows → "e UI, chuẩn bị có data mới"
append data
endInsertRows → "OK render lại đi"
~~~


##
Cac ham can phai override khi duoc ke thua QAbstractListModel de su dung

`int rowCount(const QModelIndex &parent = QModelIndex()) const override`
- Tra so luong Item trong model

`QVariant data(const QModelIndex &index, int role) const override;`
- Tra du lieu tuong ung voi tung Item

`QHash<int, QByteArray> roleNames() const override;`
- Map role → tên (dùng cho QML)

Vi du:
~~~
roles[TitleRole] = "title";
~~~
QML se dung
~~~
text: model.title
~~~