Trong bộ framework Qt (thường dùng chung với QML/C++), QNetworkAccessManager là "trái tim" của mọi hoạt động liên quan đến mạng.

Nói một cách đơn giản nhất: Nó là công cụ để gửi và nhận dữ liệu qua Internet.

## 1.  Chức năng chính
QNetworkAccessManager cho phép ứng dụng thực hiện các yêu cầu (Requests) phổ biến nhất của giao thức HTTP:
- GET: Tải dữ liệu từ Server (ví dụ: tải file JSON, lấy nội dung trang web).
- POST / PUT: Gửi dữ liệu lên Server (ví dụ: gửi form đăng ký, tải ảnh lên cloud).
- DELETE: Gửi yêu cầu xóa một tài nguyên trên Server.
- HEAD: Chỉ kiểm tra thông tin phản hồi (Header) mà không tải toàn bộ nội dung.


## 2. Cách thức hoạt động (Cơ chế Bất đồng bộ)
Điểm quan trọng nhất cần nhớ là QNetworkAccessManager hoạt động theo cơ chế Asynchronous (Bất đồng bộ).
- gửi một yêu cầu đi.
- Chương trình không bị đứng hình (freeze) để đợi kết quả.
- Khi có dữ liệu trả về hoặc có lỗi, nó sẽ phát ra một tín hiệu (Signal) báo cho bạn biết để xử lý.



## 
Một ứng dụng thì một manager network


Do not directly delete it inside the slot connected to finished(). You can use the deleteLater() function.

Tại sao? Khi tín hiệu finished() phát ra, đối tượng QNetworkReply vẫn có thể đang thực hiện các tác vụ dọn dẹp nội bộ. Nếu bạn delete reply; ngay lập tức, ứng dụng sẽ bị crash. Hãy luôn dùng reply->deleteLater();.


## Quy trình làm việc
Mọi việc diễn ra theo 3 bước cực đơn giản:
- Bước 1 - Viết địa chỉ (QNetworkRequest): Bạn lấy một cái phong bì, viết địa chỉ trang web vào đó (ví dụ: [https://google.com](https://google.com)).
- Bước 2 - Gửi thư (Hàm .get() hoặc .post()): Bạn đưa phong bì cho "Ông trưởng trạm" (Manager). Ông ấy sẽ nói: "Được rồi, tôi đã nhận, khi nào có kết quả tôi sẽ báo cho bạn".
- Bước 3 - Nhận hồi đáp (QNetworkReply): Vì gửi thư qua mạng mất thời gian (vài giây), nên bạn không cần đứng đợi. Bạn cứ đi làm việc khác. Khi thư hồi đáp về, ông ấy sẽ rung chuông (Signal finished) để bạn ra lấy đồ.



## Ứng dụng này chỉ có 2 phần

Giao diện (QML): Một cái nút bấm và một nơi hiển thị chữ.

Xử lý (C++): Dùng QNetworkAccessManager để lấy nội dung từ một trang web.

- Bước 1: Ý tưởng hoạt động
Bạn bấm nút "Lấy dữ liệu".

C++ nhận lệnh, bảo QNetworkAccessManager đi lấy dữ liệu từ [https://google.com](https://google.com).

Khi lấy xong, nó gửi nội dung đó ngược lại màn hình để bạn đọc.

- Bước 2: Code phần Xử lý (C++)
Đừng quá lo lắng về các dòng code, hãy chú ý vào các chú thích mình ghi nhé:

~~~cpp
#include <QNetworkAccessManager>
#include <QNetworkReply>

void MyApp::fetchData() {
    // 1. Tạo "Ông trưởng trạm"
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    // 2. Viết địa chỉ lên phong bì và gửi đi
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl("https://www.google.com")));

    // 3. Đợi khi nào "thư hồi đáp" về (finished)
    connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            // Lấy nội dung chữ trong thư ra
            QString data = reply->readAll(); 
            qDebug() << "Đã lấy được dữ liệu!";
        }
        // Dọn dẹp sau khi xong việc
        reply->deleteLater();
    });
}
~~~

- Bước 3: Code phần Giao diện (QML)
Sử dụng thẻ Page mà chúng ta đã nói ở trên để làm cái khung:

File: main.qml
~~~
import QtQuick
import QtQuick.Controls

ApplicationWindow {
    visible: true
    width: 400
    height: 300

    Page {
        anchors.fill: parent
        
        // Tiêu đề trang
        header: Label { 
            text: "Ứng dụng lấy dữ liệu"
            font.pixelSize: 20
            horizontalAlignment: Text.AlignHCenter
        }

        // Nút bấm ở giữa màn hình
        Button {
            text: "Bấm để lấy nội dung Web"
            anchors.centerIn: parent
            onClicked: {
                console.log("Đang yêu cầu ông trưởng trạm đi lấy đồ...")
                // Gọi hàm fetchData ở phần C++ bên trên
                networkProvider.fetchData() 
            }
        }
    }
}
~~~



## Kiến thức chưa hiểu
- URL là gì 

Dữ liệu nhận được từ web ở dạng JSON

Dùng http/https để lấy dữ liệu or dữ liệu lên trang web
- GET: lấy dữ liệu
- POST or PUT:
    - POST: thường dùng để đưa dữ liệu mới
    - PUT: thường dùng để cập nhập dữ liệu cũ
- DELETE: Xóa dữ liệu

Các giao thức khác
- FTP: chuyên dùng để truyền tải file nặng giữa các máy tính
- Websocket: Dùng khi trao đổi dữ liệu liên tục 2 chiều (như app tin nhắn, ...)
- MQTT: thường dùng cho các thiết bị thông minh 