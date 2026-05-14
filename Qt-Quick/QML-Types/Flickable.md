Đóng vai trò là một cửa sổ để nhìn nội dung lớn. Khi nội dung bên trong lớn hơn kích thước của chinh Flickable, user co the keo hoac vuot de xem cac phan bi an

## Để Flickable hoạt động
- Kích thước của Flickable (width, height): Là diện tích hiển thị thực tế trên màn hình (cái cửa sổ).
- Kích thước nội dung (contentWidth, contentHeight): Là tổng diện tích của toàn bộ nội dung bên trong.
- Điều kiện để kéo được: contentWidth > width (kéo ngang) hoặc contentHeight > height (kéo dọc).

## Các thuộc tính quan trọng
- contentWidth va contentHeight
- interactive: Cho phép or chặn tương tác của user
- boundsBehavior:
    - Flickable.DragAndOvershoot: Cho kéo quá biên rồi nảy lại. 
    - Flickable.StopAtBounds: Dừng khựng lại ở biên.
- flickableDirection: Giới hạn hướng vuốt
    - Flickable.HorizontalFlick, VerticalFlick hoặc AutoFlickDirection.
- contentX / contentY: Tọa độ hiện tại của nội dung so với cửa sổ.

~~~
    Rectangle {
        width: 320
        height: 320
        color: "#e0e0e0"
        border.color: "black"

        Flickable {
            id: myFlick
            anchors.fill: parent
            anchors.margins: 10 // Để lộ phần viền xám dễ quan sát

            // 1. Định nghĩa kích thước vùng nội dung lớn
            contentWidth: 1000
            contentHeight: 1000

            // 2. Cấu hình hành vi
            interactive: true
            boundsBehavior: Flickable.DragAndOvershoot // Hiệu ứng nảy khi chạm biên
            maximumFlickVelocity: 2500                 // Giới hạn tốc độ vuốt tối đa

            // 3. Nội dung bên trong
            Rectangle {
                width: 1000
                height: 1000
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "red" }
                    GradientStop { position: 0.5; color: "yellow" }
                    GradientStop { position: 1.0; color: "blue" }
                }

                Text {
                    text: "KÉO ĐỂ XEM THÊM"
                    font.pixelSize: 40
                    anchors.centerIn: parent
                }
            }

            // Hiển thị thanh cuộn (Scrollbar) - Tùy chọn
            ScrollBar.vertical: ScrollBar { }
            ScrollBar.horizontal: ScrollBar { }
        }
    }

~~~

Ví dụ
~~~
    Rectangle {
        width: 320
        height: 320
        color: "#e0e0e0"
        border.color: "black"

        Flickable {
            id: flick
            anchors.fill: parent
            clip: true

            // Rất quan trọng: Phải gán chiều cao nội dung bằng chiều cao của cột bên trong
            contentHeight: contentContainer.height

            Column {
                id: contentContainer
                width: flick.width // Để chiều ngang bằng với Flickable
                spacing: 20

                Rectangle { width: parent.width; height: 200; color: "red" }
                Rectangle { width: parent.width; height: 200; color: "blue" }
                Rectangle { width: parent.width; height: 200; color: "green" }
                Rectangle { width: parent.width; height: 200; color: "yellow" }
            }
        }
    }
~~~