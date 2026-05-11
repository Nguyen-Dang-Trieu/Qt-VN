QFile là lớp chuyên dụng để đọc và ghi file trong Qt. Nếu QDir dùng để quản lí thư mục, QFileInfo để xem thông tin file, thì QFile là công cụ để thao tác nội dung bên trong file đó.
   
QFile file(path of file)   
   
   *Cách sử dụng cơ bản
   - Khai báo: đưa đường dẫn file vào đối tượng
   - Mở file: Chọn chế độ mở (Chỉ đọc, chỉ ghi, ...)
   - Thao tác: đọc/ghi dữ liệu và đóng file
   
   Sau khi dùng file.open(), có các chế độ mở
   - QIODevice::ReadOnly: chỉ đọc
   - QIODevice::WriteOnyly: chỉ ghi (nếu file chưa có thì tạo file)
   - QIODevice::Append: ghi thêm vào cuối file
   - ...
   
   API:
   file.readAll() đọc toàn bộ dữ liệu của file và trả về dữ liệu thô dưới dạng bytes (QByteArray)
   file.atEnd() -> true if con trỏ đến end of file, false if còn data để read.
   file.readLine() -> Đọc cho đến khi gặp kí tự xuống dòng (\n, \r\n)
   file.read(size byte) -> Đọc số lượng byte theo size byte
   
## -------------------------------------------------------------------------------------------------------------- QDataStream
SInh ra để giúp "đóng gói" dữ liệu một cách an toàn và nhất quán, bất kể đang chạy trên Window, Mac hay Linux
  
    *Tại sao phải dùng ?
    - Xử lí byte order
    - GHi dữ liệu phức tạp: chỉ cần dùng toán tử <<. Muốn ghi một cái List hay một tấm ảnh? Cứ dùng << image là xong, nó tự lo việc chuyển ảnh thành byte
    - Tính nhất quán
    
    ** Các API thường dùng nhất
    <<: Ghi dữ liệu vào stream
    >>: Đọc dữ liệu từ stream
    setByteOrder(): Chọn ghi theo kiểu BigEndian hay LittlEndian
    setVersion(): Rất quan trọng! Chọn phiên bản Qt để đảm bảo file ghi từ Qt 5 vẫn đọc được bằng Qt6
    atEnd(): Kiểm tra xem đã hết dữ liệu để đọc chưa