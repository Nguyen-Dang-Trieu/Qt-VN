Trong lập trình, việc ta tạo 1 file.qml giống như đang tạo một bản thiết kế
- File.qml là một kiểu dữ liệu mới
- Đối tượng (Object): Chỉ được sinh ra khi bản thiết kế đó được sử dụng.

Trong QML, có 3 cách chính để tạo đối tượng từ một bản thiết kế (file.qml)
- Khai báo trực tiếp (static declaration): Khi viết tên component trực tiếp vào trong mã nguồn. Đối tượng sẽ được tạo ngay khi file QML chứa nó được load.
 - Cách dùng: Viết tên file (viết hoa chữ cái đầu) như một thẻ QML.

Ví dụ
~~~qml
// main.qml

Column {
    MyButton {
        text: "Click me"
    }
}
~~~

- Sử dụng Loader: Loader trì hoãn việc tạo đối tượng cho đến khi thực sự cần thiết (Ví dụ: khi user bấm vào một tab cụ thể)
 - Sử dụng: Gán đường dẫn file vào thuộc tính source.
 - Đặc điểm: Giúp app khởi động nhanh hơn và tiết kiệm RAM.

~~~qml
Loader {
    id: pageLoader
    source: "HeavyComponent.qml"
    active: true
}
~~~

- Tạo động bằng JavaScript (Dynamic Object Creation): Cho phép tạo đối tượng dựa trên các sự kiện xảy ra lúc ứng dụng đang chạy (Runtime). Có 2 phương pháp
 - Dùng Qt.createComponent(): Phù hợp khi ta đã có sẵn một file .qml riêng biệt.
  - 1. Tạo "Component" từ file
  - 2. Gọi hàm createObject() từ Component đó

Ví dụ
~~~qml
var component = Qt.createComponent("Sprite.qml)
if(component.status === Component.Ready) {
    var newObject = component.createObject(parentItem, {x: 100, y: 100});
}
~~~
 - Dùng Qt.createQmlObject(): Phù hợp khi ta muốn tạo nhanh một đối tượng từ một chuỗi văn bản (string)
định dạng QML ngay trong code