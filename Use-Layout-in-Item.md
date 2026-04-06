Cách dùng Layout bên trong một Item

Cách 1: anchors.fill: parent

~~~
Window {
    id: window

    // Kích thước mặc định khi vừa mở app
    width: 500
    height: 300

    visible: true // Cho phep hien thi cua so window khi chay
    title: "File Vault"


    Rectangle {
        width: 300
        height: 400
        color: "#3b5388" // Màu nền của cả khối
        border.color: "black"
        radius: 10

        ColumnLayout {
            anchors.fill: parent // Cực kỳ quan trọng: Để Layout to bằng Rectangle
            anchors.margins: 10  // Tạo khoảng cách từ lề Rectangle vào trong
            spacing: 15

            Rectangle {
                Layout.fillWidth: true; height: 50
                color: "red"
            }
            Rectangle {
                Layout.fillWidth: true; height: 50
                color: "blue"
            }

            // Thêm một cái Spacer ở cuối để đẩy các hình lên trên
            Item { Layout.fillHeight: true }
        }
    }
}

~~~

## Cách 2: Layout ôm khít lấy Rectangle (Dùng cho Popup/Tooltip)
dùng cho những trường hợp mà bạn không biết trước nội dung bên trong dài hay ngắn bao nhiêu, và bạn muốn cái khung bên ngoài tự động "co giãn" theo nội dung đó.

Hãy tưởng tượng nó giống như một cái bao thư: Lá thư bên trong càng to thì cái bao thư phải nở ra tương ứng.
~~~
Window {
    id: window

    // Kích thước mặc định khi vừa mở app
    width: 500
    height: 300

    visible: true // Cho phep hien thi cua so window khi chay
    title: "File Vault"


    Rectangle {
        color: "lightblue"
        width: layoutInside.implicitWidth + 40  // Cộng thêm padding
        height: layoutInside.implicitHeight + 40

        ColumnLayout {
            id: layoutInside
            anchors.centerIn: parent
            spacing: 10

            Text { text: "Dòng 1" }
            Text { text: "Dòng 2 dài hơn một chút" }
        }
    }
}
       
~~~