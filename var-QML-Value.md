doc: https://doc.qt.io/qt-6/qml-var.html#details

## 1. Bản chất của var (Tóm tắt lý thuyết)
Hãy tưởng tượng var là một "chiếc thùng rỗng ma thuật". Ta không cần khai báo trước nó sẽ chứa cái gì. Ta có thể ném bất cứ thứ gì vào chiếc thùng này:

- Con số: 100
- Chuỗi chữ: "Hello"
- Đúng/Sai: true / false
- Mảng (Array - Danh sách): [1, 2, 3, "Bốn"]
- Đối tượng (Object - Một nhóm thông tin): ({ ten: "An", tuoi: 20 })
- Thậm chí là một đoạn code (Function).

Note: Đối tượng được tạo thành từ cặp key-value
## 2. Hai "Cạm bẫy" chết người khi làm thực tế và Cách xử lý
Đây là phần quan trọng nhất để code không bị lỗi trong project.

### Cạm bẫy 1: Giao diện UI không chịu cập nhật 
QML cực kỳ thông minh ở chỗ: khi dữ liệu thay đổi, giao diện sẽ tự động cập nhật (gọi là Binding). Nhưng với var, nó hơi "lười".

Nếu var của bạn chứa một `Object` (VD: chiếc xe có 4 bánh) hoặc một `Array` (Danh sách). Khi thay đổi một chi tiết nhỏ bên trong nó, QML sẽ không nhận ra sự thay đổi để cập nhật màn hình.

Code lỗi (UI không đổi):

~~~
property var car: ({ wheels: 4 })

// Nếu bạn làm thế này, chữ trên màn hình vẫn báo là 4 bánh
Button {
    onClicked: {
        car.wheels = 6 
    }
}
~~~
Cách giải quyết (Thay mới toàn bộ): ta phải báo cho QML biết bằng cách gán lại bằng một chiếc xe hoàn toàn mới, hoặc "copy" nó ra, sửa đổi, rồi gán lại.

~~~
Button {
    onClicked: {
        // Cách 1: Gán lại nguyên cái Object mới
        car = ({ wheels: 6 }) 

        // Cách 2: Phổ biến khi làm việc với Mảng (Array)
        var tempCar = car      // Lấy xe ra
        tempCar.wheels = 6     // Lắp thêm bánh
        car = tempCar          // Gán ngược lại vào property (Lúc này UI mới cập nhật!)
    }
}
~~~

### Cạm bẫy 2: Lỗi dấu ngoặc nhọn {} khi tạo Object
Trong JavaScript, {} có thể mang 2 ý nghĩa: một khối code (giống if { ... }) hoặc một Object. QML hay bị nhầm lẫn cái này.

Không bao giờ viết: `property var myData: {}` (QML sẽ hiểu đây là một cục code rỗng, kết quả là dữ liệu bị undefined - lỗi trắng màn hình).

Cách giải quyết (Luôn bọc trong ngoặc đơn): Hãy biến nó thành thói quen, hễ gán Object cho var từ đầu, hãy bọc nó trong ().

~~~
property var dataRong: ({}) 
property var userTaiKhoan: ({ ten: "Nguyen Van A", tuoi: 25 })
~~~

## 3. Chiến thuật sử dụng var hiệu quả trong Project
- Khi nào **KHÔNG NÊN** dùng: Nếu biết chắc dữ liệu đó là **số**, hãy dùng `property int` hoặc `property real`. Nếu là **text**, dùng `property string`. Nếu là **true/false**, dùng `property bool`. Code sẽ chạy nhanh hơn, an toàn hơn và QML tự động cập nhật UI rất trơn tru.

- Khi nào **BẮT BUỘC NÊN** dùng: 
    - Khi cần lưu trữ một Danh sách (Array) để hiển thị lên các View như `ListView` hay `Repeater` (Ví dụ: danh sách sản phẩm, danh sách tin nhắn).
    - Khi gọi API (mạng) trả về một cục dữ liệu JSON phức tạp gồm nhiều tầng lớp. Dùng var hứng trọn cục JSON đó là tiện nhất.


## 4. Chia sẽ biến var

    Trong QML, việc chia sẻ biến var phụ thuộc vào phạm vi (scope) mà bạn khai báo chúng. Tùy vào mục đích (chia sẻ giữa các file khác nhau hay chỉ giữa các thành phần trong cùng một file), bạn sẽ có những cách tiếp cận khác nhau:

1. Chia sẻ thông qua thuộc tính (property var)
Đây là cách phổ biến nhất. Thay vì dùng var thuần túy (chỉ tồn tại trong hàm hoặc khối code), bạn khai báo nó như một thuộc tính của Component. Các Component con hoặc Component bên ngoài có thể truy cập vào.

QML
// File: MyBreadcrumb.qml
Item {
    id: root
    // Khai báo property để các file khác hoặc con của nó có thể đọc/ghi
    property var sharedPath: [] 

    Row {
        Text {
            // Truy cập từ bên trong
            text: root.sharedPath.length > 0 ? root.sharedPath.n : ""
        }
    }
}
2. Chia sẻ toàn cục bằng Singleton (QML Shared Object)
Nếu bạn muốn một biến var có thể truy cập được ở bất cứ file nào trong dự án mà không cần truyền qua lại, bạn nên dùng Singleton.

Bước 1: Tạo file GlobalData.qml:

QML
pragma Singleton
import QtQuick 2.15

QtObject {
    property var pathStack: []
    property string userName: "Admin"
}
Bước 2: Đăng ký trong file qmldir:

singleton GlobalData 1.0 GlobalData.qml
Bước 3: Sử dụng ở bất kỳ đâu:

QML
import "." // Import thư mục chứa GlobalData

console.log(GlobalData.userName) 
GlobalData.pathStack = newStack // Cập nhật từ bất cứ đâu