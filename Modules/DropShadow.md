Tạo hiệu ứng bóng đổ

Ví dụ
~~~
    Item {
        width: 400
        height: 400

        Rectangle {
            id: rectTarget
            width: 200
            height: 100
            color: "white"
            radius: 10
            anchors.centerIn: parent

            // Nội dung bên trong hình chữ nhật (nếu có)
            Text {
                anchors.centerIn: parent
                text: "Qt 5.15 DropShadow"
            }
        }

        DropShadow {
            anchors.fill: rectTarget
            horizontalOffset: 0      // Độ lệch ngang
            verticalOffset: 0        // Độ lệch dọc
            radius: 12.0              // Độ nhòe (blur) của bóng
            samples: 25              // Số lượng mẫu (càng cao bóng càng mịn nhưng nặng GPU) (samples = radius * 2 + 1)
            color: "#60000000"       // Màu bóng (nên dùng mã hex có alpha - ví dụ 50% đen)
            source: rectTarget       // ĐỐI TƯỢNG ĐƯỢC ĐỔ BÓNG
        }
    }
    ~~~