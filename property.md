# Property trong QML

property <kiểu dữ liệu> <tên thuộc tính> : <giá trị mặc định>

## Kiểu dữ liệu cơ bản
int, double, bool, string, color

- Tạo bí danh (alias) -> property alias

property change signal: mỗi property đều có một signal thay đổi tự động sinh ra: <property name>Changed

Trong QML, khi khai báo một property, hệ thống sẽ tự động sinh ra 1 signal có tên dạng <property name>Changed
- Signal này phát ra (emit) mỗi khi giá trị của property thay đổi
- Ta có thể đón nhận signal này bằng cách viết 1 handler vs cú pháp on<property name>Changed