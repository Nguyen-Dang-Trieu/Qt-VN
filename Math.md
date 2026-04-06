## Ứng dụng 1
Để bạn dễ hình dung nhất, hãy tưởng tượng bạn đang đặt một tờ giấy lên một cái mâm.
- Cái mâm là parent (màn hình ứng dụng).
- Tờ giấy là Rectangle (khung đăng nhập của bạn).

Mục tiêu của chúng ta là: "Tờ giấy phải luôn nhỏ hơn cái mâm một chút để không bị tràn mép, nhưng nếu cái mâm khổng lồ thì tờ giấy cũng chỉ to đến một mức nhất định thôi cho đẹp."

~~~
Window {
    width: 600
    height: 400
    visible: true
    title: "Ví dụ Math.min"
    color: "#ecf0f1" // Nền xám nhạt

    // Định nghĩa các hằng số tại đây
    readonly property int marginSize: 40
    readonly property int maxWidth:   400
    readonly property int maxHeight:  200

    Rectangle {
        // LUÔN NẰM GIỮA MÀN HÌNH
        anchors.centerIn: parent

        // CHIỀU RỘNG LINH HOẠT:
        // Nếu cửa sổ nhỏ hơn 480px (400 + 80), nó sẽ co lại theo cửa sổ.
        // Nếu cửa sổ lớn hơn 480px, nó sẽ đứng im ở mức 400px.
        width: Math.min(parent.width - 2 * marginSize, maxWidth)

        // CHIỀU CAO LINH HOẠT:
        // Luôn cách mép trên dưới 40px, nhưng tối đa chỉ cao 200px.
        height: Math.min(parent.height - 2 * marginSize, maxHeight)

        color: "#3498db" // Màu xanh dương
        radius: 10

        Text {
            anchors.centerIn: parent
            text: parent.width + "x" + parent.height
            color: "white"
            font.bold: true
        }
    }
}
~~~