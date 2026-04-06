# Link docs: https://doc.qt.io/qt-6/qtquick-positioning-anchors.html


anchors.fill: parent -> component con bao phủ toàn bộ component cha
anchors.centerIn: parent -> gắn component tại vị trí trung tâm component chứa nó.
anchors.horizontalCenter: parent.horizontalCenter -> Đặt đường kẻ dọc của component này trùng với đường kẻ dọc của component cha của nó

anchors.margins: x -> chừa một khoảng cách x ở cả 4 cạnh

anchors.centerIn: parent -> căn chỉnh một đối tượng vào chính giữa (trung tâm) của object cha chứa nó
Tương đương việc viết 2 dòng đồng thời
- anchors.horizontalCenter: parent.horizontalCenter
- anchors.verticalCenter: parent.verticalCenter
