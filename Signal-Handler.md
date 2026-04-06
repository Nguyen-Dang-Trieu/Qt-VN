Trong QML, khi định nghĩa một signal, Hệ thống sẽ tự động tạo ra một signal handler tương ứng. Quy tắc chuyển đổi như sau:
- Tên signal: nameOfSignal
- Tên bộ thu (handler): on + NameOfSignal(Viết hoa chữ cái đầu tiên)

Ví dụ:
- Signal: clicked -> Signal handler: onClicked
- Signal: loginSuccess -> Signal handler: onLoginSuccess


## Bài tập cơ bản
main.qml
~~~
Window {
    width: 400; height: 300
    visible: true
    title: "Test Signal Handler"

    Column {
        anchors.centerIn: parent
        spacing: 20

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

        // SỬ DỤNG COMPONENT CON
        MyButton {
            anchors.horizontalCenter: parent.horizontalCenter

            // ĐÂY LÀ BỘ THU (Signal Handler)
            onNotifyToParent: {
                // Bạn có thể dùng trực tiếp các biến truyền sang (tinNhan, soLan)
                displayLabel.text = "Cha đã nhận: " + tinNhan + " (Lần " + soLan + ")"
                displayLabel.color = "red"
                console.log("Đã thu sóng thành công!")
            }
        }
    }
}
~~~

Mybutton.qml
~~~
import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: root
    width: 200; height: 100
    color: "lightblue"
    radius: 10

    // ĐÂY LÀ KHAI BÁO SIGNAL (Tín hiệu)
    signal notifyToParent(string tinNhan, int soLan)

    Button {
        anchors.centerIn: parent
        text: "Bấm để phát tín hiệu"

        onClicked: {
            // ĐÂY LÀ LỆNH PHÁT TÍN HIỆU (Emit)
            // Chúng ta gửi kèm theo 2 dữ liệu: một chuỗi và một con số

            root.notifyToParent("Chào đại ca!", 1)
       }
    }
}

~~~