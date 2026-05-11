QDir là một lớp cực kì mạnh mẽ trong Qt dùng để thao tác với thư mục và đường dẫn. Nó cho phép ta truy cập vào cấu trúc thư mục, liệt kê tệp tin, tạo/xóa thư mục
và lấy thông tin về hệ thống file

QDir directory("C:/MyImages"); // Khoi tao mot đối tượng thư mục từ path

 I. 
 * Các đường dẫn hệ thống hữu ích
 QString home = QDir::homePath();	// thư mục home user
 QString root = QDir::rootPath();	// thư mục gốc
 QString temp = QDir::tempPath();	// thư mục tạm
 QString current = QDir::currentPath(); 	// thư mục thực thi hiện tại


  ** API thuong dung
  - QDir(dirPath).filePath(sidecarName): nối đương dẫn thư mục + tên file
  
  VD:
    + dirPath: "C/:User/Documents"
    + sidercarName: "gocryptfs.long.abc.name
   => "C/:User/Documents/gocryptfs.long.abc.name"

  - Kiêm tra tồn tại: 
    + dir.exists()
  
  - Tạo thư mục 
  dir.mkpath("path"): hàm tạo thư mục. Điểm ưu việt là khả năng tạo ra cả một chuỗi thư mục con nếu chúng chưa tồn tại
  
  VD:
    + mkdir("A"): chỉ tạo được thư mục A. Nếu muốn tạo /A/B/C mà thư mục A chưa có, thì sẽ faild
    + mkpath("A/B/C"): Nó sẽ tự kiểm tra: nếu chưa có A thì tạo A, chưa có B thì tạo B rồi mới tạo C

  - Xóa thư mục: rmdir("nam") hoặc rmpath("path")
  - Đổi tên: rename("oldName", "newName")


  II. Liệt kê nội dung bên trong (Duyệt File/Folder)
   Sử dụng hàm entryList() or entryInfoList()
   
   1. Sử dụng bộ lọc (Filter) và Sắp xếp (Sorting)
   ~~~
   QDir dir("C:/MyFolder")
   
   dir.setFilter(QDir::Files | QDir::NoDotAnDotDot); // CHỉ lấy file, không lấy các thư mục (.) và (..)
   dir.setSorting(QDir::Name | QDir::IgnoreCase);    // Sắp xếp theo tên, không phân biệt hoa thường
   
   // Lấy danhh sách tên file (QStringList)
   QStringList files = dir.entryList();
   foreach (QString filename, files) {
   	qDebug() << "Tìm thấy file: " << filename;
   }
   ~~~

   // Lấy danh sách thông tin chi tiết (QFileInfoList)
   QFileInfoList list = dir.entryInfoList();
   for(int i = 0; i < list.size(); i++)
   {
      QFileInfo fileInfo = list.at(i);
      qDebug() << ....
   }

   2. Lọc theo định dạng: ví dụ chỉ muốn tìm các file.jpg or .png
   ~~~
   QStringList filtes;
   filtes << "*.cpp" << "*.h" << "*.txt";
   dir.setNameFilters(filters);
   
   QStringList myFiles = dir.entryList();
   ~~~
   
   3. Thao tác vs đường dẫn
    - Chuyển đổi đường dẫn tuyệt đối: dir.absolutePath()
    - dir.filePath("test.txt") trả về đường dẫn đầy đủ đến file trong thư mục đó
    - Di chuyển lên thư mục cha: dir.cdUp() (tương đương vs cd .. trong linux)
    
    Ex:
    QDir dir("C:/User/Admin/Documents");
    dir.cdUp(); -> "C:/User/Admin"
    
    - Vào thư mục con: dir.cd("subdir")