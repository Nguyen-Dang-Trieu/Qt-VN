## QAbstractListModel
La mot class base de tao ra du lieu cho UI

Hieu don gian: 
~~~
C++ data (QList, DB)
      ↓
QAbstractListModel
      ↓
QML (ListView, GridView…)
~~~

Ban chat: Yeu cau tra loi 3 cau hoi
- Co bao nhieu Item: `int rowCount(...)`
- Moi Item la gi: `QVariant data(...)`
- Moi fiels ten la gi: `roleNames()`