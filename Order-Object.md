Thứ tự Ưu tiên trong một Object QML, nên sắp xếp các thuộc tính theo thứ tự từ trên xuống dưới như sau:
- ID: Luôn đặt lên hàng đầu để định danh.
- Hằng số & Thuộc tính tùy chỉnh (property): Nơi khai báo "biến" dùng cho Object đó.
- Signals: Các thông báo sự kiện tùy chỉnh.
- Hình khối cơ bản (Position & Size): x, y, width, height.
- Neo đậu (Anchors): Cách Object bám vào các đối tượng khác.
- Giao diện (Appearance): color, opacity, radius, border, font.
- Trạng thái & Hoạt họa (States & Transitions): Logic thay đổi giao diện.
- Xử lý sự kiện (Signal Handlers): Các khối onClicked, onValueChanged.
- Object con: Các thành phần nằm bên trong.

Ví dụ áp dụng thực tế

Hãy xem cách sắp xếp này giúp code "sạch" như thế nào:
~~~
QML
Rectangle {
    // 1. ID
    id: root

    // 2. Properties (Hằng số, biến)
    readonly property int marginSize: 40
    property bool isActive: false

    // 3. Signals
    signal clickedCustom()

    // Layout Properties
    Layout.

    // 4. Position & Size
    width: 400
    height: 200

    // 5. Anchors
    anchors.centerIn: parent

    // 6. Appearance
    color: isActive ? "#3498db" : "gray"
    radius: 10
    opacity: 0.9

    // 7. States (Trạng thái)
    states: [
        State { name: "pressed"; PropertyChanges { target: root; scale: 0.95 } }
    ]

    // 8. Signal Handlers (Hành động)
    onIsActiveChanged: console.log("Trạng thái mới: " + isActive)

    MouseArea {
        anchors.fill: parent
        onClicked: {
            root.isActive = !root.isActive
            root.clickedCustom()
        }
    }

    // 9. Child Objects
    Text {
        text: "Xin chào"
        anchors.centerIn: parent
    }
}
~~~