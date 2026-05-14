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
- enabled: Cho phép user có tương tác hay không

clip = true, Item sẽ cắt bỏ những component con nếu tràn ra ngoài biên giới của Item cha

4. Xử lí sự kiện
Item không tự hiển thị, nhưng có thể nhận tương tác.
 - Keys: bắt sự kiện từ bàn phím
 - Focus: để nhận sự kiện bàn phím, Item phải có focus: true

## Cách phân bố các component con
Để hiểu cách các phần tử con xếp chồng và phân bổ thì cần nằm 3 nguyên tắc cốt lõi.

1. Hệ tọa độ 

Mọi phần tử con trong một Item đều sử dụng tọa độ tương đối so với góc bên trái (0,0) của Item cha
- Trục X -> tăng dần về phía bên phải
- Trục Y -> tăng dần từ trên xuống dưới

2. Thứ tự xếp chồng

QML quyết định cái nào nằm trên hay nằm dưới dựa vào 2 tiêu chí
- Thứ tự khai báo: Phần tử nào viết bên dưới code thì sẽ nằm đè lên phần tử trước đó.
- Thuộc tính z: Có thể ghi đè thứ tự mặc định bằng thuộc tính z
    - z:1 sẽ nằm trên z:0
    - Mặc định z:0

3. Cơ chế cắt vùng nhìn
Mặc dù phần tử con thuộc về Item cha, nếu phần tử con lớn hơn or tọa độ nằm ngoài thì nó vẫn có thể hiện ra bên ngoài Item cha
-> Nếu muốn phần tử con chỉ được phép hiện trong Item cha dùng clip: true ở Item cha

~~~
    Item {
        width: 300; height: 300

        // Rect 1: Viết trước, nhưng có z: 2 nên sẽ nằm TRÊN CÙNG
        Rectangle {
            width: 100; height: 100
            color: "red"
            z: 2
        }

        // Rect 2: Viết sau Rect 1, nhưng z mặc định là 0 nên nằm DƯỚI CÙNG
        Rectangle {
            x: 30; y: 30
            width: 100; height: 100
            color: "blue"
        }

        // Rect 3: Viết cuối cùng, z mặc định là 0.
        // Nó sẽ nằm ĐÈ lên Rect 2 vì viết sau, nhưng nằm DƯỚI Rect 1 vì z thấp hơn.
        Rectangle {
            x: 60; y: 60
            width: 100; height: 100
            color: "green"
        }
    }
~~~
