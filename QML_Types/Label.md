# https://doc.qt.io/qt-6/qml-qtquick-controls-label.html

Trong QML, Lable là thẻ nâng cấp của Text

## Các thuộc tính quan trọng nhất
- text: nội dung hiển thị
- font.bold: in đậm (true/false)
- font.pixelSize: kích thước chữ tính bằng pixel
- color: màu sắc
- elide: Tự động thêm dấu ... nếu quá dài so với chiều rộng
- wrapMode: Tự động xuống dòng khi hết chỗ

### horizontalAlignment
Căn lề ngang quyết định chữ nằm ở đâu so với chiều rộng của thẻ.
- Text.AlignHCenter: căn giữa ngang
- Text.AlignLeft: căn lè bên trái
- Text.AlignRight: căn đều bên phải
- Text.AlignJustify: căn điều 2 bên


### verticalAlignment 
Căn chỉnh nội dung chữ theo chiều dọc trong phạm vi khung thẻ đó

Để dễ hình dung, hãy tưởng tượng cái Label giống như một cái hộp. Nếu cái hộp đó cao hơn kích thước thực tế của dòng chữ, ta cần quyết định xem chữ sẽ nằm sát mép trên, mép dưới hay nằm chính giữa cái hộp đó.

- Text.AlignVCenter: căn giữa dọc
- Text.AlignTop: căn sát mép trên
- Text.AlignBottom: căn sát mép dưới