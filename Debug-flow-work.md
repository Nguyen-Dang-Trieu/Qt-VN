Hãy mở DB ra xem để biết thứ gì chưa bên trong
 - Lấy mật khẩu để mở DB, hàm setUp -> newPass tỏng chatbotdb.cpp

Tìm điểm kết thúc, biết 1 hàm nào đó chắc chắn dc gọi -> Đặt breakpoint tại đó
- Khi chương trình dừng lại, hãy nhìn vào Stack TRace
- Nó sẽ cho thấy các hàm gọi nhau để dẫn đến vị trí hiện tại. Ta có thể nhấp vào từng dòng
để xem trạng thái biến tại thời điểm đó

Dùng QSignalSpy để theo dõi signal phát ra trong hệ thống

GammaRay qt