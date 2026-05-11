# https://doc.qt.io/qt-6/qml-qtqml-component.html

Thẻ component là thẻ luôn luôn có trong mỗi thẻ khác khi ta tạo một đối tượng.

Để hiểu component trong qml, ta có thể tưởng tượng nó như một công thức nấu ăn trong quyển sách.

## Lí do
Trong QML, khi ta dùng Regtangle{}, nó sẽ hiện ra ngay lập tức. Nhưng có nhiều lúc ta không muốn nó hiện ra ngay or ta muốn dùng lại một thiết kế nhiều lần. Component ra đời
- Để đóng gói: gom các thành phần vào trong một bộ khung riêng
- Trì hoãn việc khởi tạo: Chỉ khi nào cần QML mới lấy công thức ra tạo đối tượng thật. Điều này giúp tiết kiệm RAM và app mở nhanh hơn.
- Tái sử dụng: viết 1 lần dùng ở nhiều nơi

# Properties
- progress
- status
- url

# Attached Signals
- Component.onCompleted: chạy sau khi object vừa được tạo xong và hiển thị lên màn hình
- Component.onDestruction: chạy trước khi object được xóa (Ví dụ: Lưu dữ liệu trước khi xóa)

# Methods


~~~qml
    Rectangle {
        width: 100; height: 100; color: "red"

        Component.onCompleted: {
            console.log("Rectangle duoc khoi tao")
        }

        Component.onDestruction: {
            console.log("Rectangle bi huy")
        }
    }
~~~