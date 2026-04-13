ListView hoat dong dua tren mo hinh Model - View - Delegate.
- Model: la "kho du lieu" (noi dung hien thi)
- Delegate: La "ban thiet ke" (mỗi row dữ liệu se trong nhu the nao)
- View (ListView): "nguoi dieu phoi" (sap xep cac row va quan li viec scroll)

# Cac thanh phan cot loi cua ListView
De mot ListView hoat dong, can it nhat 3 yeu to:

## Model (Du lieu)
Co the dung ListModel cho du lieu tĩnh "HOẶC" ket noi voi C++ (nhu QAbstractListModel) cho du lieu phuc tap

~~~
ListModel {
   id: myModel
   ListElement { name: "Apple", color: "red"}
   ListElement { name: "Banana", color: "yellow"}
}
~~~

# Delegate (giao dien cho moi hang)
Day la cach ta define mot "item" xuat hien tren man hinh

~~~
Component {
    id: myDelegate
    Item {
        width: parent.width; height: 40
        Text { text: name; color: color } // 'name' lấy từ Model
    }
}
~~~


ListView (bo khung)
Ket noi Model va Delegate lai voi nhau

## INDEX
index la thuoc tinh được tự động tạo ra cho mỗi delegate. Nó đại diện cho số thứ tự (vị trí) của phần tử đó trong Model, bắt đầu từ 0
- Tính chất: Mỗi phần tử có một index riêng biệt và cố định (trừ khi danh sách bị sắp xếp lại hoặc xóa bớt)
- Mục đích: Dùng để xác định xem pt đó đang ở đâu, hoặc dùng để tạo hiệu ứng thị giác.

## CurrentIndex (chỉ số hiện tại của ListView)
currentIndex là một thuộc tính của bản thân ListView. Nó đại diện cho vị trí của phần tử đang được "nhắm" tới hoặc đang được chọn.
- Tính chất: Chỉ có một currentIndex duy nhất cho toàn bộ ListView tại một thời điểm. Mặc định khi mới chạy, currentIndex = 0.
- Mục đích: Dùng để quản lí tương tác của user. Khi ta nhấn vào một dòng, thường cập nhập currentIndex của ListView bằng index của dòng đó.


## Example
~~~
import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    width: 300
    height: 400
    visible: true
    title: "Học ListView QML"

    ListView {
        id: listView_root
        anchors.fill: parent
        model: fruitModel           // important
        delegate: fruitDelegate     // important

        // Thêm khoảng cách giữa các phần tử
        spacing: 5

        // Header và Footer (Option)
        header: Rectangle { width: parent.width; height: 30; color: "lightgray"; Text { text: "DANH MỤC TRÁI CÂY"; anchors.centerIn: parent } }
    }

    // 1. Dữ liệu
    ListModel {
        id: fruitModel
        ListElement { name: "Táo"; price: "20.000đ" }
        ListElement { name: "Xoài"; price: "15.000đ" }
        ListElement { name: "Sầu riêng"; price: "80.000đ" }
    }

    // 2. Thiết kế mỗi dòng
    Component {
        id: fruitDelegate
        Rectangle {
            width: listView_root.width
            height: 35
            color: index % 2 === 0 ? "#f9f9f9" : "#ffffff" // Đổi màu xen kẽ giữa các dòng
            border.color: "red"

            Row {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 20

                Text {
                    text: name // Lấy từ ListElement
                    font.bold: true
                    verticalAlignment: Text.AlignVCenter
                }
                Text {
                    text: price
                    color: "green"
                    verticalAlignment: Text.AlignVCenter
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    listView_root.currentIndex = index // Đánh dấu dòng đang chọn
                    console.log("Bạn đã chọn: " + name)
                }
            }
        }
    }
}

~~~


Trong ListModel, mỗi thuộc tính đặt bên trong ListElement (như name, price) được gọi là một Role (vai trò).
- Khi gán model: fruitModel vào ListView, ListView sẽ đọc danh sách các Role này.
- Khi delegate (bản thiêt kế) được tạo ra cho từng dòng, QML sẽ tự động truyền các Role đó vào trong ngữ cảnh của delegate đó.


## Lý do MouseArea biết name là gì mà không cần khai báo thêm là vì

1. Cơ chế "thừa kế ngữ cảnh" (Scope Inheritance)
Khi ListView tạo ra 1 hàng, nó không chỉ vẽ 1 cái hình chữ nhật lên màn hình. Nó tạo ra một vùng không gian (context) riêng cho hàng đó.
- Tất cả các con của delegate (như Row, Text, và cả MouseArea) đều nằm trong vùng không gian này.
- Dữ liệu từ ListElement (như name, price) được "bơm" trực tiếp vào vùng không gian này dưới dạng biến cục bộ.

2. Thứ tự tìm kiếm biến
Khi viết console.log(name), QML sẽ đi tìm "name" theo thứ tự
- Trong chính MouseArea: Có biến nào tên name không ? 
- Trong Model của hàng đó: ListView sẽ hỏi "Này model, ở vị trí index này, có cái gì tên là "name" không"

=> Nếu model có => MouseArea dùng luôn giá trị đó


# Tai sao lai dung ItemDelegate ma khong dung Rectangle hay Item thong thuong ?
Mac du ta co the dung mot cai Rectangle de la delegate, nhung ItemDelegate (thuoc model QtQuick.Controls) cung cap san cac tinh nang cua mot "nut bam"
nang cao cho danh sach:
 - Hovered: Tu dong biet khi nao chuot dang di chuyen qua.
 - Pressed: Biet khi nao user dang nhan giu
 - Signals: Co san cac su kien nhu onClicked, onDoubleClicked, onHold

# CAC HAM DUOC SU DUN
## append 

 append trong listview la them record moi vao trong ListModel

 Hieu don gian ListView giong nhu mo mang object 

- Truoc khi append
 ~~~
 vaultModel = [
   { name: "A", path: "/a" },
   { name: "B", path: "/b" }
]
 ~~~

- Sau khi append
~~~
vaultModel = [
   { name: "A", path: "/a" },
   { name: "B", path: "/b" },
   { name: "NewVault", path: "/new" }  ← thêm vào cuối
]
~~~

## get

Lay toan bo data cua mot object trong ListModel theo index
~~~
data = vaultModel.get(index)
~~~