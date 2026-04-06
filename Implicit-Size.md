
Làm sao để biết kích thước của Rectangle

Ví dụ
~~~
        Rectangle {
            width: displayLabel.width + 20  // Thêm khoảng trống (padding)
            height: displayLabel.height + 10
            color: "transparent"           // Nền trong suốt
            border.color: "red"            // Màu đường viền
            border.width: 2                // Độ dày viền
            radius: 5                      // Bo góc viền
            anchors.horizontalCenter: parent.horizontalCenter

            Text {
                id: displayLabel
                text: "Đang đợi tín hiệu..."
                font.pixelSize: 18
                anchors.centerIn: parent   // Căn chữ vào giữa khung hình chữ nhật
            }
        }
~~~

## Các tính toán logic
QML sẽ tính toán theo thứ tự sau:
- Cái Text tự tính kích thước của nó: Dựa vào nội dung chuỗi "Đang đợi tín hiệu..." và font chữ pixelSize: 18, QML tính ra displayLabel rộng khoảng 150px và cao 22px (con số này thay đổi tùy màn hình).

- Cái Rectangle tính toán dựa trên Text:
    - width = 150 (của Text) + 20 = 170px.
    - height = 22 (của Text) + 10 = 32px.