# Item
Là thành phần cơ bản nhất (base type) cho tất cả các object đồ họa trong QML. Ta có thể xem nó như là một khung xương trống rỗng: Nó có vị trí, kích thước nhưng không có hình dáng (màu sắc, đường viền). Được sinh ra để giải quyết 3 vấn đề chính sau.

## Làm thùng chứa "container" 
Khi ta muốn gom nhiều thành phần (như Text, Image, ...) lại với nhau để di chuyển hoặc ẩn hiện cả cụm, ta cần một vật chứa
- Vấn đề: Nếu dùng Rectangle thì tốn tài nguyên để vẽ màu nền (dù ta để transparent)
- giải quyết: Item cực kì nhẹ vì hk có logic vẽ đồ họa, giúp tối ưu hiệu năng khi chỉ cần gom nhóm.

## Định nghĩa "không gian" và "tọa độ"
Item cung cấp các thuộc tính hình học cơ bản mà mọi vật thể đều cần:
- Vị trí & kích thước: x,y, width, height.
- Neo giữ (Anchors): Giups các con của nó bám vào lề hoặc căn giữa.
- Phân lớp (Z-order): Quyết định cái nào nằm trên, cái nào nằm dưới

## Goc Root cho các component tùy chỉnh
Khi tạo mọt file QML mới (ví dụ MyButton.QML), ta thường bắt đầu bằng Item.

Tại sao?

Nó cho phép user dùng component đó  toàn quyền quyết định bên trong có gì mà không bị áp đặt các thuộc tính thị giác. Nó đóng vai trò như là một giao diện trừu tượng.