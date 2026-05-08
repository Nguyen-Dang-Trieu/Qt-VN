Tham khảo: https://doc.qt.io/qt-6/qtqml-javascript-dynamicobjectcreation.html

# 1 Lý thuyết
Để tạo một đối tượng từ file.qml ta cần đi qua 3 bước

### Bước 1: Tạo component (Qt.createComponent)

Đây là bước nạp "bản thiết kế" từ file vào bộ nhớ. Nó chưa tạo ra thực thể, chỉ mới đọc file.

Cú pháp: `const comp = Qt.createComponent("Path/To/File.qml")`

### Bước 2: Tạo Object (createObject)
Đây là lúc xây dựng một object từ bản thiết kế

Cú pháp: `var obj = comp.createObject(parent, { property: value })`
- parent: Đối tượng cha thường là màn hình chính or root
- props: Một object chứa các giá trị muốn truyền vào trang đó ngay khi nó vừa khởi tạo

### Bước 3: Kết nối sự kiện (connect)
- Vì đối tượng này tạo ra bằng code -> Không thể sử dụng cú pháp onPressedYes: { ...} như bình thường được
- Phải dùng connect để kết nối một signal của đối tượng đó vs hàm xử lí.


# Tại sao cần dùng cách này thay vì khai báo tĩnh
- Quản lý bộ nhớ: Nếu ứng dụng có 100 loại Popup khác nhau mà ta khai báo sẵn hết, ứng dụng sẽ cực kỳ nặng. Dùng cách này, hiện cái nào thì tốn RAM cái đó, đóng lại là hết.
- Tính linh hoạt: Có thể quyết định tạo đối tượng nào dựa trên điều kiện logic phức tạp lúc runtime.

## Ví dụ
~~~qml
//main.qml

import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")


    id: mainWindow

        Button {
            text: "Hiện Popup động"
            anchors.centerIn: parent
            onClicked: {
                // Bước 1: Tạo component
                const component = Qt.createComponent("MyPopup.qml")

                // Bước 2: Tạo object và truyền property 'message'
                // Chúng ta dùng mainWindow làm parent
                var popupObj = component.createObject(mainWindow, {
                    "message": "Chào bạn, đây là thông báo!",
                    "x": 100, "y": 100 // Đặt vị trí xuất hiện
                })

                // Bước 3: Kết nối signal 'accepted' với hành động cụ thể
                popupObj.accepted.connect(function() {
                    console.log("Người dùng đã bấm OK!")
                    popupObj.destroy() // Tự hủy sau khi xong việc
                })
            }
        }

}

~~~

~~~qml
MyPopup.qml

import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: root
    width: 200; height: 150
    color: "lightgrey"
    border.color: "black"

    // Định nghĩa các Signal để bên ngoài có thể "connect" vào
    signal accepted()

    // Định nghĩa property để bên ngoài truyền dữ liệu vào
    property string message: "No message"

    Column {
        anchors.centerIn: parent
        spacing: 10
        Text { text: root.message }
        Button {
            text: "OK"
            onClicked: root.accepted() // Phát tín hiệu khi bấm nút
        }
    }
}
~~~


## ---------------
Để hiểu tại sao QML lại "thông minh" đến mức tự hiểu các tham số này, chúng ta cần chia chúng thành hai loại: Thuộc tính có sẵn và Thuộc tính tự định nghĩa.

### 1. x và y là gì?Trong QML, hầu hết các thành phần giao diện (như Rectangle, Item, Button, Page) đều kế thừa từ một lớp cha gọi là Item.
- x: Là tọa độ theo trục ngang (từ trái sang phải).
- y: Là tọa độ theo trục dọc (từ trên xuống dưới).

Gốc tọa độ `(0, 0)` nằm ở góc trên bên trái của parent. Khi bạn truyền "x": 100, "y": 100, bạn đang ra lệnh cho QML: "Hãy đặt cái Popup này cách mép trái của thằng cha 100 pixel và cách mép trên 100 pixel".

### 2. message là gì?
message không phải là từ khóa hệ thống. Nó là một Property (thuộc tính) do bạn tự đặt tên trong file MyPopup.qml.

Hãy nhìn lại file MyPopup.qml mà tôi đã ví dụ ở trên, bạn sẽ thấy dòng này:
~~~qml
QMLproperty string message: "No message"
~~~
Dòng này chính là cách bạn "khai báo" với QML rằng: "Này, cái Popup của tôi có một biến tên là message kiểu chuỗi nhé!". Nếu bạn đổi tên trong file là property string thongBao, thì khi tạo object bạn phải truyền vào là "thongBao": "...".

### 3. Làm sao QML hiểu được để gán vào? (Cơ chế Property Mapping)
Cơ chế này hoạt động giống như việc bạn điền vào một biểu mẫu (form). Khi bạn gọi createObject(parent, props), hệ thống QML sẽ thực hiện một quy trình "so khớp" như sau:
- Quét danh sách: QML nhìn vào object props bạn gửi qua (là { "x": 100, "message": "..." }).
- Tìm kiếm: Nó chạy vào file MyPopup.qml và hỏi: "Trong này có ai tên là x không? Có ai tên là message không?".
- Gán giá trị:
    - Nó tìm thấy x (vì là thuộc tính mặc định của Item), nó gán x = 100.
    - Nó tìm thấy message (vì bạn đã khai báo property string message), nó gán message = "Chào bạn...".
    
Bỏ qua hoặc báo lỗi: Nếu bạn truyền một cái tên lạ hoắc (ví dụ: "abc": 123) mà trong file .qml không khai báo property int abc, QML sẽ báo lỗi ở console: “Cannot assign to non-existent property abc”.

Ví dụ thực tế để bạn dễ hình dung

Hãy tưởng tượng MyPopup.qml là một chiếc xe hơi bạn tự thiết kế:
- Mặc định xe nào cũng có Bánh xe (x, y, width, height).
- Bạn thiết kế thêm một đặc điểm riêng là Màu sơn (property string mauSon).

Khi bạn "đặt hàng" tạo xe (createObject), bạn gửi kèm yêu cầu:
~~~
JavaScript{
    "x": 50,           // Đặt xe ở vị trí 50
    "mauSon": "Đỏ"     // Sơn xe màu đỏ
}
~~~
Hệ thống sẽ hiểu: "À, bánh xe thì lắp vào chỗ cũ, còn màu sơn thì lấy màu Đỏ mà người dùng vừa yêu cầu để sơn lên".

Lưu ý quan trọng

Nếu bạn không truyền props khi tạo object, QML sẽ lấy giá trị mặc định mà bạn đã viết trong file .qml.

Ví dụ: Nếu trong file bạn viết property string message: "Thông báo mặc định", mà lúc createObject bạn không truyền message, thì nó sẽ hiển thị chữ "Thông báo mặc định".