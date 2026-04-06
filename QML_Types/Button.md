doc: https://doc.qt.io/qt-6/qml-qtquick-controls-button.html

# Thuộc tính
- text: nội dung hiển thị trên nút
- enabled: cho phép user được quyền tương tác hay không (true/false)
- flat: true, nút sẽ không có khung nền (chỉ hiện chữ), khi move chuột vào mới hiện nền mờ
- opacity: điều chỉnh độ trong suốt 
- icon.source: Thêm biểu tượng cho nút
- display: quyết định hiện chữ, icon, hay cả hai.

# Method
- onClicked: Đây là signal handler mặc định 
- onPressed: Ngay khi vừa nhấn
- onReleased: Khi thả ra
- onDoubleClicked: Khi bấm nhanh 2 lần liên tiếp
- onPressAndHold: Khi bấm và giữ một lúc lâu

## onClicked
Hướng dẫn cách dùng
- Cách dùng cơ bản:
~~~
Button {
    text: "Bấm vào tôi"
    onClicked: console.log("Người dùng đã bấm nút!")
}
~~~
- Khi có nhiều dòng code thì ta cần viết trong Block: "{ ... }"
~~~
Button {
    text: "Đăng nhập"
    onClicked: {
        var user = userField.text
        var pass = passField.text
        
        if (user === "admin" && pass === "123") {
            stackView.replace(vaultPage)
        } else {
            errorLabel.text = "Sai tài khoản hoặc mật khẩu!"
        }
    }
}
~~~
- Phát tín hiệu
~~~
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
Ta có thể hiểu đơn giản là component cha (Rectangle) sẽ phát đi một signal có form: ("Chào đại ca!", 1) 


# Tùy chỉnh giao diện
Theo default, button của Qt khá cổ điển, để thiết kế hiện tại hơn ta cần ghi đè 2 thành phần:
- contextItem: nội dung bên trong cái nút
- background: vẻ bề ngoài của cái nút đó

Ví dụ:
~~~
Button {
    id: customBtn
    text: "Login to Vault"
    
    // 1. Tùy biến chữ (Content)
    contentItem: Text {
        text: customBtn.text
        font.bold: true
        color: customBtn.pressed ? "#cccccc" : "white" // Đổi màu khi bấm
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    // 2. Tùy biến khung nền (Background)
    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 40
        color: customBtn.pressed ? "#1a5fb4" : "#2f81f7" // Hiệu ứng đổi màu nền
        border.color: "#ffffff"
        border.width: customBtn.visualFocus ? 2 : 0 // Hiện viền khi dùng phím Tab
        radius: 8
    }
}
~~~