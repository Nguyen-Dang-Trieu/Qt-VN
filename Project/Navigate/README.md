

## Làm thế nào để ListView lấy được data trong m_nodes
Để lấy được dữ liệu từ m_nodes (C++) và hiển thị lên ListView (QML), Qt sử dụng một cơ chế gọi là **Model/View Architecture**.

Cụ thể, ListView không "xem" trực tiếp biến `m_nodes`, mà nó hỏi thông qua các hàm Standard Interface (giao diện chuẩn) mà ta đã ghi đè (override) trong file .h.

Dưới đây là quy trình 3 bước mà QML lấy dữ liệu:

#### Bước 1: Hỏi số lượng (rowCount)

Khi gán model: fileNodeModel, ListView sẽ gọi hàm `rowCount()` này đầu tiên để biết phải tạo ra bao nhiêu "hàng" (delegate) trên màn hình.

~~~cpp
int rowCount(const QModelIndex &parent = QModelIndex()) const override { 
    return m_nodes.count(); // Trả về số lượng phần tử đang có trong m_nodes
}
~~~
#### Bước 2: Định nghĩa "Tên gọi" (roleNames)
Vì m_nodes chứa các struct NodeItem với nhiều trường (id, name, type), QML cần biết từ khóa nào ứng với dữ liệu nào. Hàm này tạo ra các "nhãn" để ta có thể dùng bên QML.

~~~cpp
roles[NameRole] = "nodeName"; // C++ nói: "Khi QML gọi 'nodeName', hãy đưa NameRole cho nó"
~~~

#### Bước 3: Lấy dữ liệu chi tiết (data)
Đây là hàm quan trọng nhất. Sau khi biết số lượng hàng, ListView sẽ chạy một vòng lặp để lấy dữ liệu cho từng hàng.
- Ví dụ: Khi vẽ hàng thứ 0, QML sẽ yêu cầu: "Cho tôi nodeName của hàng 0".
- C++ xử lý:
~~~
QVariant data(const QModelIndex &index, int role) const override {
    // 1. Tìm đúng vị trí phần tử trong m_nodes dựa vào index.row()
    const NodeItem &item = m_nodes[index.row()]; 

    // 2. Kiểm tra QML đang hỏi "nhãn" (role) nào
    switch (role) {
        case NameRole: 
            return item.name; // Trả về chuỗi tên (ví dụ: "Documents")
        case TypeRole: 
            return item.type; // Trả về số 1 hoặc 2
        // ...
    }
}
~~~

## Câu hỏi: Làm sao biết giá trị index = ? khi truyền gọi data()
### Phía QML: ListView là "Người điều phối"
Khi ta gán model: fileNodeModel, ListView thực hiện các bước tự động sau:
- Nó gọi hàm rowCount() trong C++. Giả sử C++ trả về là 3.
- ListView sẽ tự động tạo ra một vòng lặp (vòng lặp này chạy ngầm bên trong mã nguồn của Qt).
- Với mỗi vòng lặp, nó sẽ tạo ra một Index tương ứng:
    - Lần 1: Tạo index có giá trị 0.
    - Lần 2: Tạo index có giá trị 1.
    - Lần 3: Tạo index có giá trị 2.

### Phía C++: Cách index được tạo ra để gửi vào hàm data()
Trong C++, tham số const QModelIndex &index mà ta thấy không phải là một số nguyên đơn thuần, mà là một Object chứa thông tin vị trí.

Khi ListView muốn vẽ hàng số 0, nó sẽ thực hiện lệnh (giả mã): `QModelIndex idx = model->index(0, 0);`

Sau đó nó mới gọi: `model->data(idx, NameRole);`