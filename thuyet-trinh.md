"Xin chào sếp và các anh. Hôm nay em xin trình bày về kiến trúc bảo mật của dự án CryptVault.

Tại sao chúng ta lại cần một kiến trúc mới? Như các anh đã biết, các giải pháp như Gocryptfs hay 
Cryptomator hiện đang làm rất tốt nhiệm vụ bảo vệ nội dung file. Tuy nhiên, qua quá trình nghiên 
cứu, em nhận thấy các ứng dụng này vẫn còn tồn tại một vài vấn đề mà tin tặc có thể 
khai thác mà không cần phải giải mã dữ liệu:

Slide truncate
"Thưa sếp và các anh, đây là minh họa cụ thể cho lỗ hổng Truncate (cắt xén dữ liệu).

[Chỉ vào phần trên]
Thông thường, khi mã hóa một file, ứng dụng sẽ chia nhỏ dữ liệu thành từng khối (ví dụ 32 KiB) và 
thêm các đoạn mã xác thực (28B Metadata) cho mỗi khối đó. Cơ chế này giúp đảm bảo nếu nội dung 
bên trong một khối bị sửa đổi, ứng dụng sẽ biết ngay.

[Chỉ vào mũi tên đỏ Truncate]
Tuy nhiên, điểm yếu nằm ở đây: Tin tặc có thể thực hiện một cuộc tấn công vật lý đơn giản là cắt 
bỏ đoạn của file có kích thước là một block

[Chỉ vào phần dưới có dấu X đỏ]
Vì các khối dữ liệu này mã hóa/giải mã hoàn toàn độc lập, Cho nên khi người dùng mở lại file, ứng dụng 
vẫn giải mã các  khối còn lại một cách bình thường. Kết quả là người dùng nhận được một file bị 
thiếu dữ liệu mà không nhận được bất kỳ cảnh báo hệ thống nào về việc dữ liệu file đã bị xâm phạm.

Điều này cực kỳ nguy hiểm nếu đó là một file thực thi (.exe) bị mất đoạn code cuối, hoặc một file 
tài liệu quan trọng bị mất đi phần kết luận."

# Swap file
1. Cách Cryptomator "Đeo mặt nạ" cho tên File & Folder
"Đầu tiên, làm sao Cryptomator giấu được tên file của chúng ta?
Thay vì giữ nguyên tên Bao_cao.pdf, nó sẽ dùng một thuật toán mã hóa đặc biệt
(AES-SIV – dạng mã hóa có kiểm tra tính toàn vẹn) để biến cái tên này thành một chuỗi ký tự vô 
nghĩa, rồi gắn thêm đuôi .c9r.

Nhưng để ngăn kẻ gian lén bốc file từ thư mục này vứt sang thư mục khác, Cryptomator dùng một mẹo 
rất hay:
- Nó lấy ID của thư mục chứa file đó (dirId) làm ngữ cảnh mã hóa.

Nghĩa là:

Cùng một tên file
Nhưng ở hai thư mục khác nhau
→ sẽ cho ra hai tên mã hóa hoàn toàn khác nhau

Vì vậy, nếu bạn copy file .c9r sang thư mục khác:
→ hệ thống sẽ không giải mã được tên nữa
→ và coi file đó là không hợp lệ."


2. Mê cung thư mục và 'Bản đồ' dir.c9r
"Vậy làm sao hệ thống biết đâu là file, đâu là folder khi tất cả đều có đuôi .c9r?

Rất đơn giản:
- Nếu nó là File: Nó cứ nằm im đó và chứa dữ liệu.
- Nếu nó là Folder: Khi mở cái file .c9r đó ra, bên trong sẽ có một file đặc biệt tên là dir.c9r.

File này đóng vai trò như một tấm bản đồ.
 
Bên trong nó chứa một ID (dirId) đại diện cho thư mục đó.

Từ ID này, hệ thống sẽ thực hiện hash (băm) và sau đó tạo ra một đường dẫn có nhiều cấp. Đây mới 
chính là nơi lưu trữ thật của toàn bộ dữ liệu được mã hóa của thư mục chính. Nhờ cấu trúc này 
mà cấu trúc thư mục trở nên rất khóa xác định


“Tóm lại, khi một thư mục được mã hóa bằng Cryptomator, nó không còn tồn tại dưới dạng một thư mục
duy nhất như ban đầu.
Thay vào đó, hệ thống tách nó thành hai phần:

Một phần là thư mục trong vùng dữ liệu (thường nằm trong d/), dùng để lưu trữ toàn bộ nội dung đã 
được mã hóa.
Phần còn lại là một thư mục đại diện với đuôi .c9r. Bên trong thư mục này có một file đặc biệt 
tên là dir.c9r, chứa một ID của thư mục.

Từ ID này, hệ thống sẽ tính toán ra đường dẫn tương ứng đến nơi lưu trữ dữ liệu mã hóa thực sự.

“Tuy nhiên, cách thiết kế này cũng tiềm ẩn một điểm yếu: đường dẫn đến nơi lưu trữ dữ liệu mã hóa 
phụ thuộc trực tiếp vào folder ID.

Vì vậy, nếu kẻ tấn công tráo đổi nội dung của hai file dir.c9r giữa các thư mục, ví dụ important 
và unimportant, thì hệ thống sẽ bị ‘trỏ nhầm’ sang vùng dữ liệu của thư mục khác.

Khi đó, quá trình giải mã vẫn diễn ra bình thường, nhưng dữ liệu thu được không còn là dữ liệu 
gốc của thư mục ban đầu, mà là dữ liệu của một thư mục khác.

hay