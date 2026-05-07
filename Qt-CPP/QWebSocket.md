
Các mục 
- Public Functions (Hàm công khai): Đây là các phương thức bạn có thể gọi trực tiếp từ đối tượng `QWebSocket`. Chúng đại diện cho các hành động và khả năng truy xuất thông tin của đối tượng.
- Public Slots (Các Slot công khai): Trong cơ chế Signals & Slots của Qt, Slot là những hàm có thể được kết nối với một Signal.
- Signals (Tín hiệu): Đây là mục cực kỳ quan trọng đối với lập trình hướng sự kiện. Signals là những thông báo mà đối tượng phát ra khi có một sự kiện cụ thể xảy ra.
- Static Public Members (Thành viên tĩnh công khai): Đây là những hàm thuộc về chính lớp QWebSocket chứ không thuộc về một đối tượng cụ thể nào.

Cách dùng: Gọi qua tên lớp, ví dụ: QWebSocket::maxIncomingMessageSize().

Ý nghĩa: Thường dùng để lấy các giới hạn hoặc cấu hình mặc định chung cho tất cả các kết nối WebSocket trong hệ thống (như kích thước gói tin tối đa được phép).