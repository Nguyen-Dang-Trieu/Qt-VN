
Mô hình kế thừa:

TextField <- TextInput <- Item

## Phương thức
### Kế thừa
forceActiveFocus(): Chủ động đưa con trỏ chuột vào ô nhập dữ liệu

### Bản thân

## Thuộc tính 
### Kế thừa
- text: Là dữ liệu thô (raw) khi user nhập vào
- displayText: dữ liệu hiển thị mà user thấy được

Mỗi thuộc tính đều phát ra signal khi giá trị được thay đổi

Vd: text -> phát ra signal textChanged -> nhận được ở slot onTextChanged

- enabled: Cho phép user nhập và sửa đổi văn bản nếu là true, ngược lại thì false
- validator: bộ kiểm soát những kí tự cho phép user được nhập vào text

## Signal
### Kế thừa
- accepted(): Khi user xác nhận nhấn Enter, ... -> slot onAccepted xử lí