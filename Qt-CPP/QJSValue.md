
# Bản chất
JavaScript là một ngôn ngữ không cần khai báo kiểu dữ liệu cứng nhắc (một biến có thể là số, chuỗi
or object). Khi ta muốn đưa giá trị đó qua C++ (ngôn ngữ rất chặt chẽ về kiểu dữ liệu), cần một đối tượng trung gian để có thể chứa hết tất cả chúng => QJSValue làm việc đó.

Có thể chứa:
- Các kiểu cơ bản: undefined, null, bool, number, string
- Các kiểu phức tạp: Array, Object, Function
- Các đối tượng Qt: QObject*


## Method
### isObject(): nếu QJSValue là một đối tượng, kết quả là true
Ta dùng khi ta truyền dữ liệu là một cấu trúc phức hợp chứa cặp key:value chứ không phải là một giá trị đơn lẻ.
- Ta có thể truy cập những thuộc tính bên trong bằng .property()

Như thế nào được xem là một Object trong js?

Một đối tượng được định nghĩa bằng {}
- Plain Object: var obj = { name: "GhostRider", age: 25 };
- Mảng (Array): Một điểm thú vị là trong JS, mảng cũng là một Object đặc biệt. Vì vậy, nếu dữ liệu là [1, 2, 3], cả isObject() và isArray() đều trả về true.
- Instance của một Class: Ví dụ một đối tượng được tạo từ new Date() hoặc một Component QML.
