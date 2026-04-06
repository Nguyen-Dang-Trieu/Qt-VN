Bình thường, các hàm trong class C++ là "nội bộ", QML không có quyền động vào. Nếu muốn từ giao diện (QML) bấm một cái nút và bắt phải chạy 1 hàm nào đó, ta có 2 cách
- Biến hàm đó thành một slot
- Thêm macro Q_INVOKABLE khi khai báo hàm

Điều này sẽ thông báo với hệ thống Meta - Object của Qt rằng "Hãy đăng kí hàm này vào list công khai để cho QML dùng"