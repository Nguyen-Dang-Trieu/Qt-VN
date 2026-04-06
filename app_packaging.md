Bước 1: Khi tạo dự án ta cần ở bảng configu build ta cần phải tích vào "Release"
Bước 2: Chọn release -> build.

Sau đó, ta tìm vào thư mục buidl -> release trong dự án, Kiểm tra trong thư mục đó phải chứa file.exe

Bước 3: Mở Qt Command Prompt (bắt buộc) -> Nhảy tới thư mục relase chứa file.exe mà ta đã kiểm tra bên trên
Bước 4: Gõ
~~~
windeployqt --qmldir d/ D:path_dir name_app.exe
~~~
Bước 5: Nhấn Enter và chờ đợi, sau đó ta nhấn vào file.exe đó thử kiểm tra app có chạy bình thường được chưa => Nếu mọi thứ đều ok thì quá trình thành công