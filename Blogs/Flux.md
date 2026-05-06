Bài đọc thêm: https://vinascript.com/ngon-ngu-lap-trinh/flux-la-gi-cach-xay-dung-ung-dung-voi-flux-don-gian.html

https://github.com/benlau/quickflux

Flux là một kiến trúc một chiều flow, giúp quản lí luồng trong ứng dụng

Nguyên tắc cơ bản:
- Actions: Các sự kiện diễn tả ý định của user or app.
- Dispatcher: là trung gian phân phát các actions đến store
- Stores: Kho dữ liệu/trạng thái
 - Stores giữ trạng thái của ứng dụng và chịu trách nhiệm xử lí các actions
 - Khi dữ liệu thay đổi, stores phát ra tín hiệu để component QML lắng nghe và update giao diện.
- View:
 - Component QML hiển thị dữ liệu từ store
 - Thay vì trực tiếp thay đổi giao diện, View gửi action va lắng nghe từ store




## Cách sử dụng QuickFlux
Bước 1: Định nghĩa Action Types

Tạo file ActionTypes.qml. Đây là nơi liệt kê tất cả các hành động có thể xảy ra trong app.
~~~
import QtQuick 2.0
import QuickFlux 1.1

pragma Singleton // Nên để là Singleton để gọi mọi nơi

KeyTable {
    property string increment
    property string decrement
}
~~~
Với KeyTable có sẽ tự gán giá trị cho thuộc tính bằng tên của thuộc tính đó

Kết quả:
- ActionTypes.increment sẽ có giá trị là chuỗi "increment".
- ActionTypes.decrement sẽ có giá trị là chuỗi "decrement".


Bước 2: Tạo AppActions

Tạo file AppActions.qml. UI sẽ gọi các hàm ở đây thay vì can thiệp trực tiếp vào dữ liệu.
~~~
import QtQuick 2.0
import QuickFlux 1.1
import "." // Import ActionTypes

ActionCreator {
    // Không cần viết hàm dispatch nữa, chỉ cần khai báo signal
    // Tên signal phải trùng với tên property trong ActionTypes.qml
    
    signal increment()

    signal decrement()
    
    // Nếu bạn muốn truyền thêm dữ liệu (ví dụ tăng bao nhiêu đơn vị)
    // signal increment(int amount)

    // Định nghĩa tham số nào, tham số đó sẽ chui vào 'message'
    signal updateUsername(string name, var updatedAt)
}
~~~

#### Giải thích:
dispatch(type, message): giống như một tham số
- type: Tiêu đề của bức thư giúp xác định thuộc loại nào để gửi đúng nơi nhận
- message (Nội dung thư): Đây là một object chứa các thông tin chi tiết đi kèm


Bước 3: Stores

Store sẽ nghe các Action từ Dispatcher và cập nhật giá trị. File MainStore.qml
~~~
import QtQuick 2.0
import QuickFlux 1.1
import "." 

Store {
    property int count: 0 // Trạng thái của ứng dụng

    Filter { // đóng vai trò là một màn lọc
        type: ActionTypes.increment // Đây là bộ lọc chỉ lắng nghe những thông tin có nhãn increment, các tín hiệu khác chạy qua thì hk xử lí
        onDispatched: count++; 
    }

    Filter {
        type: ActionTypes.decrement
        onDispatched: count--;
    }
}
~~~

onDispatched: đây là một signal handler. Khi một hành động vượt qua được "màng lọc" ở trên, code bên trong khối này sẽ chạy.
- Điều quan trọng nhất: Bên trong onDispatched, ta có quyền truy cập trực tiếp vào biến message

Ví dụ khi message có dữ liệu
~~~
// Trong MainStore.qml
property string userName: "Guest"

Filter {
    type: ActionTypes.updateUsername
    onDispatched: {
        // 'message' chính là { name: newName, ... } ở trên
        userName = message.name; 
        console.log("Thời gian cập nhật:", message.updatedAt);
    }
}
~~~



Kết nối vào giao diện: main.qml
~~~
import QtQuick 2.15
import QtQuick.Controls 2.15
import "." 

ApplicationWindow {
    visible: true
    width: 200
    height: 200

    // Khai báo Store và Actions (thường dùng Singleton hoặc đặt ở gốc)
    MainStore { id: mainStore }
    AppActions { id: appActions }

    Column {
        anchors.centerIn: parent
        spacing: 10

        Text {
            text: "Số hiện tại: " + mainStore.count
            font.pixelSize: 20
        }

        Button {
            text: "+"
            onClicked: appActions.increment()
        }

        Button {
            text: "-"
            onClicked: appActions.decrement()
        }
    }
}
~~~