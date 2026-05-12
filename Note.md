Sơ đồ mối quan hệ
- ChatBotConversation (1) <-> (N) ChatBotMessage: Một cuộc hội thoại chứa nhiều tin nhắn
- ChatBotMessage (1) <-> (N) ChatBotFile: Một message có thể đi kèm nhiều file (doc, ảnh, video, ...)
- ChatBotConversation (1) <-> ChatBotFile: Một cuộc hội thoại cũng có thể quản lí danh sách
file
- ChatBotDB: Đóng vai trò là "Manager" (Quản lí) điều phối toàn bộ việc lưu trữ và két nối cơ sở
dữ liệu

ChatBotDB: DB tổng quát -> file chatbotdb.db

Các table (Kế thừa từ  AbstractDbo)
- ChatBotConversation: Danh sách các cuộc trò chuyện
- ChatBotMessage: Danh sách tất cả các message
- ChatBotFile: Danh sách các file đã gửi/nhận



## Chi tiết vai trò trong class
### ChatBotConversation: Đại diện cho một phiên bản chat (session)
- m_identity: để định danh duy nhất
- ListMessagesRelation để giữ các danh sách ChatBotMessage
- m_last_mesg: tin nhắn cuối cùng
...


### ChatBotMessage
Đại diện cho từng dòng tin nhắn cụ thể trong cuộc hội thoại
- m_conv_identity: Khóa ngoại (Foreign Key) trỏ về ChatBotConversation
- m_FileRelation: Danh cách các file đính kèm ChatBotFile thuộc về tin nhắn này

Chứa các trạng thái của chatBot
- m_think_data: dữ liệu suy nghĩ 
- m_plan
- m_status
...

### ChatBotFile
Quản lý các tệp tin đa phương tiện.

Liên kết:
- m_msg_identity: Trỏ về tin nhắn chứa file này
- m_conv_identity: Trỏ về cuộc hội thoại chứa file này

Lưu trữ thông tin
- m_local_path: đường dẫn máy tính
- m_server_uri: link server
- m_file_size


## Conversation (Cuộc hội thoại)
- Vị trí: Chính là các mục ở Thanh sidebar bên trái.
- Đặc điểm: Mỗi khi bạn bấm "New Chat", một dòng mới sẽ được chèn vào table ChatBotConversation
- Dữ liệu lưu trữ:
    - m_alias: Tên tiêu đề cuộc chat
    - m_last_mesg: Tin nhắn cuối cùng
    - m_updated_at: Để sắp xếp cuộc hội thoại nào mới nhất lên đầu sidebar 

## Message
Một conversation sẽ có nhiều message
- Phân biệt được message dựa vào `m_author` là "User" và "AI" => Chỉ là suy đoán
- m_conv_identity: liên kết message này thuộc về conversation nào
- m_body - body: Nội dung câu trả lời hiển thị cho bạn
- m_think_data - cột think_data: Nội dung suy nghĩ ngầm của AI
- m_time - time: thời điểm AI phản hồi
