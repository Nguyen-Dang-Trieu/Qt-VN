Trong framework Qt, QTextStream là một công cụ mạnh mẽ và tiện lợi được thiết kế chuyên biệt để thao tác với luồng dữ liệu văn bản (text). Điểm khác biệt lớn nhất so với thao tác đọc/ghi trực tiếp trên QFile là khả năng tự động quản lý bộ đệm (buffering), mã hóa ký tự (encoding), và xử lý chuẩn hóa ký tự xuống dòng giữa các hệ điều hành khác nhau.

Nói đơn giản, ta có thể tưởng tượng là QTextStream giống như một đường ống và kết nối với kho chưa là một
file, dữ liệu sẽ thông qua đường ống đó và vào file


## 1. Đọc và ghi File văn bản
Khi kết hợp QFile và QTextStream giúp việc thao tác với các file cấu hình, file log, hoặc export dữ liệu trở nên an toàn và dễ đọc hơn.

Ví dụ
~~~cpp
QFile file(path)
file.open(QIODevice::WriteOnly | QIODevice::Text)

QTextStream out(&file)
out.setCodec("UTF-8"); // Nếu là Qt5 còn Qt6 thì mặc định

out << "Dữ liệu A";
out << "Dữ liệu B";
~~~

Hỗ trợ việc xử lí từng dòng dữ liệu trong file
~~~cpp
QFile file(filePath);
if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream in(&file);
        
    while (!in.atEnd()) {
        QString line = in.readLine();
        // Xử lý từng dòng dữ liệu tại đây
    }
    file.close();
}
~~~

## API

### QIODevice *QTextStream::device() const
Dùng để kiểm tra xem QTextStream hiện tại có làm việc với thiết bị hay vùng nhớ nào
- Trả về một con trỏ tới QIODevice (có thể là QFile, QTcpSocket, QBuffer, ...)
- Khi nào dùng: Muốn kiểm tra xem thử QTextStream có kết nối với thùng chứa nào chưa

### void QTextStream::setDevice(QIODevice *device)
Dùng để thay đổi nguồn đang giao tiếp với QTextStream
- QTextStream sẽ gọi flush() trước khi thay đổi device cũ sang device mới.