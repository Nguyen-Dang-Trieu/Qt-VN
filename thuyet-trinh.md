"Xin chào sếp và các anh. Hôm nay em xin trình bày về kiến trúc bảo mật của dự án CryptVault.

Tại sao chúng ta lại cần một kiến trúc mới? Như các anh đã biết, các giải pháp như Gocryptfs hay 
Cryptomator hiện đang làm rất tốt nhiệm vụ bảo vệ nội dung file. Tuy nhiên, qua quá trình nghiên 
cứu, em nhận thấy các ứng dụng này vẫn còn tồn tại một vài vấn đề mà tin tặc có thể 
khai thác mà không cần phải giải mã dữ liệu:

Vấn đề đầu tiên
"Thưa sếp và các anh, đây là minh họa cụ thể cho lỗ hổng Truncate (cắt xén dữ liệu).

[Chỉ vào phần trên]
Thông thường, khi mã hóa một file, ứng dụng sẽ chia nhỏ dữ liệu thành từng khối (ví dụ 32 KiB) và 
thêm các đoạn mã xác thực (28B Metadata) cho mỗi khối đó. Cơ chế này giúp đảm bảo nếu nội dung 
bên trong một khối bị sửa đổi, ứng dụng sẽ biết ngay.

[Chỉ vào mũi tên đỏ Truncate]
Tuy nhiên, điểm yếu nằm ở đây: Tin tặc có thể thực hiện một cuộc tấn công vật lý đơn giản là cắt bỏ đi một khối (block) dữ liệu ở cuối file.

[Chỉ vào phần dưới có dấu X đỏ]
Vì các khối dữ liệu này được giải mã hoàn toàn độc lập, nên khi người dùng mở lại file, ứng dụng vẫn giải mã các khối còn lại một cách bình thường. Kết quả là người dùng nhận được một file bị thiếu hụt dữ liệu mà không nhận được bất kỳ cảnh báo nào..."

Điều này cực kỳ nguy hiểm nếu đó là một file thực thi (.exe) bị mất đoạn code cuối, hoặc một file 
tài liệu quan trọng bị mất đi phần kết luận."

"Tiếp theo là một lỗ hổng tinh vi hơn: Tấn công tráo đổi (Swap).

# Swap file
Đầu tiên, Cryptomator giấu tên file và thư mục bằng cách mã hóa AES-SIV và dùng chính ID của thư mục cha (dirID) làm ngữ cảnh mã hóa. Nghĩa là bị 'khóa chặt' vào thư mục chứa nó, không thể copy bậy bạ sang thư mục khác.

"Vậy làm sao hệ thống biết đâu là file, đâu là folder khi tất cả đều có đuôi .c9r?

Rất đơn giản:
- Nếu nó là File: Nó cứ nằm im đó và chứa dữ liệu.
- Nếu nó là Folder: Khi mở cái file .c9r đó ra, bên trong sẽ có một file đặc biệt tên là dir.c9r.

File này đóng vai trò như một tấm bản đồ.
 
Bên trong nó chứa một ID (dirId) đại diện cho thư mục đó.

Từ ID này, hệ thống mới tính toán ra một đường dẫn trỏ tới vùng dữ liệu thực sự. Nhờ việc tách rời 'Tên thư mục' và 'Vùng dữ liệu' này, hệ thống giấu được toàn bộ cấu trúc cây thư mục của người dùng.

“Tuy nhiên, cách thiết kế này cũng tiềm ẩn một điểm yếu: đường dẫn đến nơi lưu trữ dữ liệu mã hóa 
phụ thuộc trực tiếp vào folder ID.

Vì vậy, nếu kẻ tấn công tráo đổi nội dung của hai file dir.c9r giữa các thư mục, ví dụ quan trọng
và thư mục rác, thì hệ thống sẽ bị ‘trỏ nhầm’ sang vùng dữ liệu của thư mục khác.

Lúc này, hệ thống bị 'trỏ nhầm'. Khi ta click vào thư mục Quan trọng, quá trình giải mã vẫn diễn ra trơn tru, nhưng nội dung hiện lên lại là của thư mục Rác. Tin tặc đã trao đổi toàn bộ bộ một thư mục mà không cần biết mật khẩu."

"Từ những lỗ hổng đó, dự án CryptVault V2 của em không chọn cách vá lỗi, mà xây dựng một kiến trúc hoàn toàn mới với 3 trụ cột:

Thứ nhất là Chiến lược Bucket (Thùng chứa): Thay vì ánh xạ từng file với kích thước lớn nhỏ khác nhau, hệ thống của em gom tất cả dữ liệu nhét vào các thùng chứa cố định có kích thước 128 MiB. Khi hacker nhìn vào, chúng chỉ thấy những khối 128 MiB giống hệt nhau. Dữ liệu của người dùng hoàn toàn 'tàng hình'.

Thứ hai là Lớp mã hóa Slice: Bên trong các Bucket đó, dữ liệu được băm nhỏ thành các Slice. Bất kỳ một nỗ lực tráo đổi vị trí (Swap) hay cắt xén (Truncate) nào, dù chỉ là 1 byte, cơ chế xác thực đa tầng sẽ phát hiện ra ngay lập tức và báo động đỏ cho người dùng, chặn đứng việc đọc dữ liệu sai.

Cuối cùng là Quản trị Database tập trung: Em loại bỏ hoàn toàn cơ chế dùng file dir.c9r rải rác làm bản đồ. Toàn bộ cấu trúc cây thư mục được làm phẳng và lưu trữ trong một Database SQLite duy nhất được mã hóa nghiêm ngặt. Điều này giúp ngăn chặn triệt để việc hacker tráo đổi con trỏ thư mục, đồng thời giúp ứng dụng truy xuất đường dẫn với tốc độ siêu tốc."