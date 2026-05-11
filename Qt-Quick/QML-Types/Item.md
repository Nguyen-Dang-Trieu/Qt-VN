doc: https://doc.qt.io/qt-6/qml-qtquick-item.html#anchors.horizontalCenter-prop

Item chính là nền móng để tạo nên những thứ khác

1. Item chính là một "Container"
Vì Item không có hình ảnh hiển thị nên người ta dùng nó để nhóm các đối tượng khác lại với nhau
 - Tính đóng gói: có thể tạo ra một component phức tạp để có thể di chuyển cả khối


## Thuộc tính
Mọi thứ kế thừa từ Item đều có những thuộc tính như này
- Hình học: x,y, width, height, z (độ ưu tiên hiển thị lớp dưới)
- Anchors
- Transform: scale, rotation, opacity (độ trong suốt)
...

- visible: Cho phép component ẩn/hiện trên UI, default: `true`
    - false: ẩn hoàn toàn, không nhận được bất cứ sự kiện gì. Các component con cũng bị ẩn theo.

clip = true, Item sẽ cắt bỏ những component con nếu tràn ra ngoài biên giới của Item cha

4. Xử lí sự kiện
Item không tự hiển thị, nhưng có thể nhận tương tác.
 - Keys: bắt sự kiện từ bàn phím
 - Focus: để nhận sự kiện bàn phím, Item phải có focus: true

