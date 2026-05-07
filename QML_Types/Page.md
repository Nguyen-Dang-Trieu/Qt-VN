Trong ngôn ngữ QML (Qt Modeling Language), thẻ Page là một thành phần điều khiển (Control) cực kỳ phổ biến, thường được sử dụng để xây dựng cấu trúc giao diện cho các ứng dụng đa màn hình. Nó nằm trong module QtQuick.Controls.

Dưới đây là những công dụng và đặc điểm chính của thẻ Page:

1. Cấu trúc hóa giao diện (Header, Footer, Content)
Công dụng lớn nhất của Page là nó cung cấp sẵn các "vị trí" để đặt các thành phần cố định. Thay vì ta phải tự tính toán vị trí, Page chia sẵn:
- header: Khu vực cố định ở trên cùng (thường dùng cho Thanh tiêu đề hoặc Menu).
- footer: Khu vực cố định ở dưới cùng (thường dùng cho các Tab bar hoặc thanh trạng thái).
- contentItem: Khu vực ở giữa dành cho nội dung chính, tự động co giãn theo phần còn lại của trang.

2. Quản lý nội dung tự động
Khi bạn đặt một thành phần vào trong Page, nó sẽ tự động tính toán không gian còn trống sau khi đã trừ đi phần Header và Footer. Điều này giúp giao diện luôn cân đối mà không cần viết quá nhiều code căn chỉnh (anchors).

3. Kết hợp hoàn hảo với StackView
Trong các ứng dụng di động hoặc ứng dụng có nhiều lớp màn hình, Page thường được dùng làm "con" của StackView.