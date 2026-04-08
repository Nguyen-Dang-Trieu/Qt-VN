https://doc.qt.io/qt-6/qml-qtquick-droparea.html

Tóm tắt lý thuyết
DropArea hiểu đơn giản là một vùng tàng hình dùng để "hứng" các đối tượng bị kéo thả vào nó. Bản thân nó không có giao diện, ta thường phải đặt nó lồng bên trong một Rectangle hoặc Item để tạo ranh giới.

3 Trạng thái cốt lỗi signal cần nhớ
- onEntered: Kích hoạt khi ta vừa rê chuột (đang giữ đồ vật) vào vùng DropArea. Dùng để kiểm tra xem đồ vật đó có hợp lệ không (ví dụ: chỉ cho phép file ảnh).
- onExited: Kích hoạt khi ta rê chuột ra khỏi vùng. Dùng để tắt các hiệu ứng hover.
- onDropped: Kích hoạt khi ta nhả chuột. Đây là lúc ta lấy dữ liệu (đường dẫn file) để xử lý.


Thuộc tính (Properties) hay dùng:
- containsDrag: (True/False) Trả về True nếu đang có vật thể lơ lửng trên nó. Cực kỳ hữu ích để đổi màu nền tạo hiệu ứng thị giác cho người dùng.
- keys: Dùng để lọc. Nếu set keys: ["text/plain"], nó sẽ từ chối nhận file hình ảnh.