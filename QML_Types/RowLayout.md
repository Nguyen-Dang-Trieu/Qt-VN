Cơ chế của RowLayout
- RowLayout hoạt động giống như một hàng ghế trong rạp chiếu phim. Người nào (hay Item ) được khai báo trước
thì sẽ chiếm vị trí bên trái nhất, Item tiếp theo sẽ ngồi vào trị trí bên cạnh

Layout.preferredWidth: parent.width * 0.4 -> Chiếm 40% chiều rộng của layout cha
Layout.preferredHeight: x -> Tôi muốn ngăn kéo này cao đúng kích thước x
Layout.fillWidth: true -> Chiếm hết kích thước của Width còn lại
Layout.fillHight: true -> Chiếm toàn bộ kích thước còn lại của chiều cao

implicitHeight: Chiều cao tự nhiên


##
Trong một RowLayout, chiều cao của hàng thường được quyết định bởi Item cao nhất
- Những Item thấp hơn, nếu không có lệnh căn chỉnh, mặc định sẽ bám sát mép trên y = 0
- Khi thêm Qt.AlignVCenter, item đó sẽ tự tính toán để khoảng trống phía trên và phía dưới bằng nhau, giúp nó nằm chính giữa chiều dọc của hàng đó.

Đây là thứ tự ưu tiên để mà RowLayout sử dụng để quyết định chiều rộng Item
- 1. Chiều rộng mặc định (implicitWidth): Nếu ta không viết 1 dòng code nào liên quan đến Width, Item sẽ chiếm diện tích vừa đủ để hiển thị nội dung của nó
- 2. Chiều rộng ưu tiên (Layout.preferredWidth): Nếu ta dùng code này thì Layout sẽ cố dành riêng cho Item đúng 200pixel
## Example
~~~
Window {
    id: window

    // Kích thước mặc định khi vừa mở app
    width: 500
    height: 200

    visible: true // Cho phep hien thi cua so window khi chay
    title: "File Vault"


    RowLayout {
        width: 500
        height: 100 // Chiều cao hàng cố định là 100
        spacing: 0

        // Item 1: Logo rất cao (100px)
        Rectangle {
            width: 100; height: 100
            color: "blue"
            // Thằng này cao gần bằng hàng nên nhìn luôn cân đối
        }

        // Item 2: Nút bấm thấp (30px) - KHÔNG CĂN GIỮA
        Button {
            text: "Home"
            Layout.preferredHeight: 30
            // Kết quả: Nút này sẽ bị treo lơ lửng ở sát mép trên cùng, nhìn rất xấu
        }

        // Item 3: Nút bấm thấp (30px) - CÓ CĂN GIỮA
        Button {
            text: "Settings"
            Layout.preferredHeight: 30
            Layout.alignment: Qt.AlignVCenter
            // Kết quả: Nút này sẽ nhảy xuống giữa, thẳng hàng với tâm của Logo
        }

        Item {
            Layout.fillWidth: true
        }
    }
}
~~~

- Nếu không có Item thì, qt sẽ tự động chia đều và canh chỉnh các component sao cho hợp lí nên các component
con sẽ tách nhau ra
- Có Item thì do Item chiếm hết phần sau rồi, nên có cần chỉnh theo đúng kích thước trong code


## 
Trong Qt quick layouts, khi đã dùng RowLayout và ColumnLayout thì không nên dùng
- anchors.fill và anchors.margin
- Phải dùng Layout.fillWidth và Layout.margin