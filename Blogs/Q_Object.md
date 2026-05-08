Trong C++, một class thông thường chỉ là các cấu trúc dữ liệu và hàm. Tuy nhiên, khi thêm macro Q_OBJECT vào đầu một class (kế thừa từ QObject), ta đang "mở khóa" toàn bộ sức mạnh của framework Qt.

1. Cơ chế Signals và Slots (Quan trọng nhất)
Đây là "đặc sản" của Qt để các đối tượng giao tiếp với nhau. Nếu không có Q_OBJECT, trình biên dịch Meta-Object (MOC) sẽ không thể tạo ra code cần thiết để kết nối tín hiệu từ đối tượng này sang hàm của đối tượng khác.

Signal: Thông báo một sự kiện vừa xảy ra (ví dụ: clicked()).

Slot: Hàm phản hồi lại sự kiện đó.