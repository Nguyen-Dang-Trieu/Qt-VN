Giống như một chiếc túi có thể co giãn chứa dữ liệu thô (raw)

QByteArray rawdata
QByteArray aad(8, 0); -> MỘt mảng 8 byte, các bytes có giá trị = 0

VD: Dữ liệu "thô" (raw data)
- Số 1234 thô sẽ là "04 D2" (hệ Hex) hoặc 00000100 11010010 (Binary).
- Chữ "Hello" thô là: "48 65 6C 6C 6F"

    ** API
    rawdata.append(): thêm dữ liệu vào cuối mảng
    rawdata.size(): trả số lượng byte đang có
    rawdata.data() hay rawdata.constData(): Lấy con trỏ trỏ thẳng vào vùng nhớ thô
    rawdata.mid(pos, len): Cắt một đoạn dữ liệu từ vị trí pos với độ dài len
    rawdata.clear(): xóa sạch mọi thứ trong túi để dùng lại
    rawdata.reserve(size byte): Đặt trước kích thước của một vùng byte theo "size byte"
    rawdata.toHex(): Hien raw duoi dang hex
    rawdata.constData(): lấy một con trỏ kiểu "const char* trỏ thẳng vào vùng nhớ của mà QByteArray đang giữ
     Vd: Nếu QByteArray là thùng chứa dữ liệu thì, constData sẽ trỏ vào vị trí đầu tiên của thùng chứa dữ liệu đó
    
    rawdata.left(count bytes): Lay count bytes tu dau mang
    rawdata.right(count bytes): Lay count bytes tu cuoi mang
       VD: 
         QByteArray ba = "ERR_404"
         QByteArray res = ba.left(3) // ERR
         res = ba.right(3) // 404