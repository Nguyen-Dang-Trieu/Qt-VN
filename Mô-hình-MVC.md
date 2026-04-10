Mô hình MVC: https://itviec.com/blog/mvc-la-gi/

Trong mô hình MVC chuẩn, Model không bao giờ được biết View là ai. Tuy nhiên, "không giao tiếp" ở đây nghĩa là Model không được gọi trực tiếp các hàm của View, chứ không phải là dữ liệu không thể truyền tới View.

Dưới đây là cách chúng "giao tiếp" mà vẫn giữ được khoảng cách:

### 1. Cơ chế "Phát tin đồn" (Signals & Slots)
Thay vì Model cầm tay View và nói: "Này, cập nhật số 10 đi", Model sẽ đứng giữa phòng và hét lên: "Dữ liệu của tôi vừa thay đổi thành 10 rồi nhé!".

`Model`: Phát ra một **signal** (ví dụ: dataChanged). Nó không quan tâm ai đang nghe.

`View`: "Đăng ký" (connect) để nghe tín hiệu đó. Khi nghe thấy tín hiệu, View tự cập nhật giao diện của chính mình.

### 2. Vai trò của Controller 
Controller đóng vai trò như một người quản gia:
- View nhận thao tác người dùng (click chuột) -> Báo cho Controller.
- Controller xử lý logic và ra lệnh cho Model cập nhật.
- Model cập nhật xong -> Phát ra tín hiệu thông báo.
- View nhận tín hiệu đó (hoặc thông qua Controller điều phối) -> Cập nhật hiển thị.