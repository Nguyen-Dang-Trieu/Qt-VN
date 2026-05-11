  QFileInfoList list = directory.entryInfoList();

### QFileInfoList
	QFileInfoList thuc chat chi la cai ten khac (alias) cua QList<QFileInfo>
	QFileInfoList la chứa "hồ sơ" của tất cả mọi thư năm trong thư mục đó, bao gồm cả file và thư mục con (Trong OS, thư mục là 1
	loại file đặc biệt)
	
	Vi la mot QList, co the thao tac nhu mot mang thong thuong
	
 * Thuoc tinh
 - QDir::AllEntrie : Tat ca cac file va thu muc
 - QDir::NoDotAndDotDot : An di 2 thu muc dieu huong . va .. 

~~~
QDir myDir("C:/Documents");
QFileInfoList list = myDir.entryInfoList();

list.size(): so luong item trong danh sach
list.at(index) lay phan tu tai vi tri index
list.isEmpty() kiem tra danh sach co rong hay khong