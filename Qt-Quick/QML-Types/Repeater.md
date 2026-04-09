doc: https://doc.qt.io/qt-6/qml-qtquick-repeater.html

## 1. Repeater là gì?
Repeater là một bộ tạo bản sao. Nó dùng một cái "khuôn" (delegate) để nhân bản ra nhiều đối tượng giống nhau dựa trên một nguồn dữ liệu (model).

Đặc điểm quan trọng: Nó tạo ra toàn bộ các item ngay lập tức khi được load (khác với ListView chỉ tạo item khi bạn cuộn tới).

## 2. Các thành phần chính
- model (define = var): Nguồn dữ liệu (có thể là một con số, một danh sách chuỗi, hoặc mảng đối tượng).
- delegate: Hình dáng và logic của từng item được sinh ra (ví dụ: Rectangle, Text...).
- index: Thứ tự của item đó trong danh sách (bắt đầu từ 0).
- modelData: Dữ liệu cụ thể của item đó (nếu model là một mảng).


## 3. Cách áp dụng thực tế
### Trường hợp 1: 
Tạo số lượng cố định (Model là một con số). Nếu user chỉ muốn vẽ 5 cái vòng tròn:

~~~
Row {
    spacing: 10
    Repeater {
        model: 5
        Rectangle {
            width: 20; height: 20; radius: 10
            color: "blue"
        }
    }
}
~~~

### Trường hợp 2: 
Dùng với mảng đối tượng. Đây là cách bạn dùng dữ liệu [{"n": "Root", "id": 1}] để hiển thị thanh điều hướng
~~~
Row {
    spacing: 5
    Repeater {
        model: pathStack // Sử dụng biến bạn đã hỏi ở trên
        
        // Đây là "cái khuôn"
        Text {
            required property var modelData // Lấy dữ liệu của từng object
            text: modelData.n + " > "       // Hiển thị chữ "Root >"
            color: "black"
            
            MouseArea {
                anchors.fill: parent
                onClicked: console.log("Bạn đã nhấn vào ID:", modelData.id)
            }
        }
    }
}