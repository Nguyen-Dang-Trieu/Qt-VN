https://doc.qt.io/qt-6/properties.html

Trong Qt, Q_PROPERTY là một macro dùng để khai báo một property(thuộc tính) cho một class. Nó giúp kết nói giữa logic code C++ và giao diện QML or Meta-object của Qt

VD:
~~~
Q_PROPERTY(QString message READ message NOTIFY messageChanged)
~~~
- QString message: tên thuộc tính là mesage và kiểu dữ liệu là QString
- READ message: Chỉ định hàm sẽ lấy giá trị. Khi hệ thống muốn đọc dữ liệu nó sẽ gọi hàm message()
- NOTIFY messageChanged: Chỉ định một signal tên là messageChanged. Khi giá trị thuộc tính thay đổi, ta cần phải emit signal này để những bên theo dõi (client) cập nhập những thứ cần thiết