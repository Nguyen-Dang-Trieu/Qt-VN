# https://doc.qt.io/qt-6/qml-qtquick-layouts-columnlayout.html

## Column Layout giải quyết vấn đề gì
Trong QML cơ bản, chúng ta có Column. Tuy nhiên, Column chỉ đơn thuần là xếp chồng các item lên nhau. ColumnLayout giải quyết những vấn đề phức tạp hơn:
- Tự động co giãn (Resizing): Các item bên trong có thể tự lấp đầy khoảng trống khi cửa sổ ứng dụng phóng to/thu nhỏ (Layout.fillWidth, Layout.fillHeight).
- Căn lề linh hoạt (Alignment): Mỗi item trong cột có thể có cách căn lề riêng (trái, phải, giữa) mà không phụ thuộc vào nhau.
- Quản lý kích thước thông minh: Cho phép thiết lập kích thước tối thiểu (minimum), tối đa (maximum) và kích thước ưu tiên (preferred) cho từng thành phần.
- Hỗ trợ giao diện Responsive: Giúp giao diện thích ứng với nhiều độ phân giải màn hình khác nhau một cách tự động.

##
Hãy tưởng tượng ColumnLayout giống như một cái tủ ngăn kéo đứng
- Mỗi khi thêm vào một component, nó sẽ tự động đưa vào một ngăn kéo mới.
- Một cái ngăn kéo chỉ chứa duy nhất một món đồ.
- Chiều rộng của ngăng kéo luôn bằng chiều rộng cái tủ.


Trong ColumnLayout, mỗi khi thêm một thành phần (Button, Rectangle, ...), Layou sẽ tự tạo ra một ô (cell) tàng hình bao quanh item đó.
- Độ rộng của các cell này mặc định bằng với độ rộng của cả ColumnLayout
## Thuộc tính
### Thuộc tính của ColumnLayout (Item cha điều khiển tổng thể bố cục layout)
- spacing: khoảng cách pixel giữa các item con
- layoutDirection: hướng sắp xếp (mặc định từ Trái sang phải). Nếu chỉnh Qt.RightToLeft, các item căng lề bên trái sẽ nhảy sang phải

### Attached Properties (Item con tự xác định lại vị trí của mk trong Item cha)
Gọi chúng thông qua cú pháp: Layout.thuoc_tinh
- Căn lề (Layout.alignment): 
    - Qt.AlignLeft: sát mép trái
    - Qt.AlignRight: sát mép phải
    - Qt.AlignVCenter (trục Y): đặt item vào chính giữa chiều cao cell mà Layout đã tạo cho item.
    - Qt.AlignHCenter (trục X): đặt item vào chính giữa chiều ngang của cell mà Layout đã tạo cho item.
    - Qt.AlignCenter: = Qt.AlignVCenter + Qt.AlignHCenter

- Khả năng lắp đầy: Nếu set = true thì item đó sẽ lắp đầy toàn bộ không gian còn trống của Layout
    - Layout.fillWidth: true/false (mặc định)
    - Layout.fillHeight: true/false (mặc định)
- Kích thước ưu tiên (Layout.preferredWidth / Layout.preferredHeight)


Example:
~~~
Window {
    id: window

    // Kích thước mặc định khi vừa mở app
    width: 300
    height: 200

    visible: true // Cho phep hien thi cua so window khi chay
    title: "File Vault"


    ColumnLayout {
        width: 200
        height: 400 // Chiều cao tổng rất lớn
        spacing: 10

        Rectangle {
            width: 200; height: 50
            color: "red"
            // Mặc định: Nằm trên cùng bên trái của "ô" thứ 1
        }

        Rectangle {
            width: 100; height: 50
            color: "blue"
            Layout.alignment: Qt.AlignRight // Căn giữa theo chiều ngang (rất hay dùng)
        }

        Rectangle {
            width: 100; height: 50
            color: "green"
            // Nếu ô này được kéo giãn ra (do Layout.fillHeight)
            // thì AlignVCenter mới thực sự rõ rệt
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignCenter
        }
    }
}

~~~