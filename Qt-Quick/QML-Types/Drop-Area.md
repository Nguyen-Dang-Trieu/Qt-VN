https://doc.qt.io/qt-6/qml-qtquick-droparea.html

drop là  một đối tượng (Object) chứa toàn bộ thông tin về "món hàng" mà người dùng vừa thả vào app
 - Khi onDropped xảy ra, QML sẽ tạo ra đối tượng này và ném vào hàm để ta xử lí
 - Bên trong drop (object) chứa những gì
   + Tạo độ nơi chuột thả xuống (x,y)
   + Định dạng dữ liệu (text, images hay file)
   + Đường dẫn urls.
   
- hashUrls() trả về true. Nếu user vừa thả là danh sách các đường dẫn file or thư mục từ máy tính.

- urls: là một mảng (Array/List) - đóng vai trò là kho chứa đường dẫn 
   + Cái kho này chứa bao nhiêu URL cũng được, tùy thuộc vào số lượng file or thư mục mà user quét chọn rồi kéo vào.
     Ex: 
       Kéo 1 file: urls.length là 1
       Kéo 100 file: urls.length là 100
       
   + Thứ tự của các URL trong danh sách phụ thuộc vào OS.
   
   Ví dụ minh hoa thực tế:
   
   Giả sử ta có 3 file trong máy: A.txt, B.jpg, C.pdf. Boi den ca 3 va keo vao app
    - urls[0]: file:///.../A.txt
    - urls[1]: file:///.../B.jpg
    - urls[2]: file:///.../C.pdf
    
   Lưu ý: Khi ta kéo một thư mục (trong thư mục đó có nhiều file) vào vùng DropArea, chỉ có URL của thư mục gốc thôi không bao gồm các URL của file chứa bên trong thư mục đó
    
  

## ------

* Khi hasUrls đã là true, bạn truy cập vào trường urls để lấy dữ liệu. Vì user có thể kéo cùng lúc nhiều file, nên urls luôn là một danh sách (Array/List).
 - Thả 1 file: urls có 1 phần tử.
 - Thả 10 file: urls có 10 phần tử.

* "Đường dẫn" này có hình dạng thế nào?

Đường dẫn trong urls không phải là đường dẫn thuần túy kiểu Linux (/home/user/...), mà nó là định dạng URL.

Nó sẽ luôn có tiền tố: file://

Loại	Dạng trong urls[i]			Dạng C++ cần (để đọc file)
----       ------------------    		---------------------------------
Linux	file:///home/GhostRider/data.txt		/home/GhostRider/data.txt
Windows	file:///C:/Users/Admin/data.txt		C:/Users/Admin/data.txt


Thuc chat, khi ta keo mot file or folder vao vung nay thi OS chi gui cac metadata
 - URL: Dia chi file nam o dau tren disk
 - Toa độ (x,y): Ta đang thả nó vào chỗ nào trên màn hình
 - Định dạng (MIME): Đây là file hay là chữ
 
 
* Vậy làm sao để có được "Nội dung" 
- Vì DropArea chỉ có ta cái "địa chỉ", nên muốn đọc nội dung của file để làm gì đó => Thực hiện thông qua C++

* Khi nào thì nó mới chứa "Nội dung" thực sự ?
Đối tượng drop chỉ chứa nội dung trực tiếp trong 2 TH nhẹ:
- drop.text: Khi ta bôi đen một đoạn văn chữ và kéo vào. Chữ rất nhẹ nên hệ thống đưa thẳng nội dung vào luôn
- drop.color: Khi kéo một màu sắc từ bảng màu
