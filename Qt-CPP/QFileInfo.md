QFileInfo giup ta xem mọi thông tin của file or thư mục, tùy thuôc vào đường dẫn mà ta truyền vào

Ví dụ:
QFileInfo checkInfo(path)

 - path = "/home/GhostRider/Projects/CryptVault" -> fileName() = CryptVault (tên thư mục)
 - path = "/home/GhostRider/Projects/CryptVault.pdf" -> fileName() = CryptVault.pdf (tên của file)


  * Khoi tao co ban
  #include <QFileInfo>
  
  QFileInfo info("path cua file")
  
  vd: QFileInfo info("C:/Data/report.pdf");
  
  * Sau khi co doi tuong info, ta co the lay thong tin cuc ki nhanh chong
   1. Ve ten goi
     info.fileName() -> "report.pdf" 
     info.baseName() -> "report"
     info.suffix() -> "pdf"
     info.absoluteFilePath() -> "C:/Data/report.pdf"
     info.absolutePath() -> "C:/Data"
   
   2. Ve dac diem
     info.size() -> tra ve size tinh bang bytes (chia 1024 de lay KB)
     info.exists() -> File nay co thuc su ton tai tren o cung khong ?
     info.isFile() -> Day co phai la file khong
     info.isDir() -> Day co phai la thu muc khong
     
   3. Ve thoi gian & quyen
     info.lastModified() -> ngay gio sua doi cuoi cung (Tra ve kieu QDateTime)
     info.isReadable()/info.isWritable() -> co quyen doc hay ghi file nay khong ?
   
Example
  1. Cách chuyển file từ thư mục A sang thư mục B
  ~~~
  QString outputdir = "D:/Backup";
  QFileInfo info("C:/Photos/vacation.jpg")
  
  QString opath = QDir(outputdir).filePath(info.fileName());
  // Ket qua opath = "D:/Backup/vacation.jpg"
  ~~~
