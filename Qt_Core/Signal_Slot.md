Bài đọc gốc: https://doc.qt.io/qt-6/signalsandslots.html#slots

- Slot có thể nhận ít tham số hơn signal
- Một class signal sẽ không quan tâm class slot nào xử lí.
- signal và slot kết nối với nhau bằng connect

 Các class kế thừa từ QObject đều có signal và slot
 Các object phát ra signal nó không quan tâm object nào khác có thể nhận dc tính hiệu hay không -> Đảm bảo tính đóng gói

 Slot có thể được sử dụng nhận signal - nó cũng là một hàm thành viên thông thường
 Cũng giống như signal, một slot cũng không biết dc là có signal nào liên kết với nó hay không -> Điều này đảm bảo các thành phần độc lập.

 - Có thể kết nối một signla với nhiều slot or ngược lại 1 slot với nhiều signal
 - Có thể kết nối 2 signal với nhau, và điều này sẽ gây ra hiện tượng là khi signal 1 được phát ra thì signal 2 cũng sẽ làm điều như vậy

 ## Signal
 - Signal được phát ra khi state bên trong thay đổi.
 - Hàm kiểu public có thể phát ra bất cứ ở đâu -> Recommend chỉ nên phát ra ở class định nghĩa signal or những class con 

Cơ chế của signal và slot hoàn toàn độc lập so với GUI.
Những câu lệnh ở phía sau lệnh emit thì sẽ dược thực thi khi mà slot trả về kết quả

Tuy nhiên có 1 th đặc biệt hơn khi sử dụng queued connections.
- Các câu lệnh ở phía sau emit sẽ được thi ngay lập tức
- Các slot sẽ được xếp vào hàng đợi và sẽ xử lí sau đó

Queue connections thường xảy ra khi
- Signal phát ở thread A
- Slot ở thread B

Nếu nhiều slots mà kết nối vs 1 signal, thì thứ tự thực thi slot sẽ tuân theo thứ tự kết nối

signal sẽ được tạo tự động bởi moc và không được triển khai trong cpp
 ## Slots
 - Slots vẫn là một hàm của class, nên nó tuân theo các luật C++ thông thường
Vd:
~~~cpp
class A {
private slots:
    void foo();
};

A a;
a.foo();   // ❌ LỖI – vì foo() là private
~~~

 - Khi slot được gọi thông qua signal-slot connections.
  + Qt không phân biệt là public, private hay protected
  + Chỉ cần đã connect -> slot sẽ được gọi
~~~cpp
class B {
signals:
    void triggered();
};

class A {
private slots:
    void foo();   // private slot
};

connect(&b, &B::triggered, &a, &A::foo);
emit b.triggered();  // ✅ foo() vẫn chạy

~~~