QtConcurrent::blockingMap là một hàm tiện lợi trong Qt áp dụng một hàm nhất định (hoặc functor/lambda) cho từng phần tử trong một vùng chứa song song và chặn cho đến khi tất cả các phần tử đã được xử lý.

Đây là một phần của mô-đun Qt Concurrent, cung cấp các API cấp cao để lập trình song song mà không cần xử lý trực tiếp các luồng, mutex hoặc đồng bộ hóa cấp thấp.

## Hiểu đơn giản
- map -> áp dụng function lên từng phần tử
- concurrrent -> chạy song song nhiều thread
- blocking -> chờ hoàn thành hết rồi mới chạy tiếp

VD
~~~c
#include <QtConcurrent>

QList<int> list = {1, 2, 3, 4, 5};

QtConcurrent::blockingMap(list, [](int &x) {
    x *= 2;
});

// KẾT QUẢ
list = {2, 4, 6, 8, 10}
~~~

Cách nó hoạt động 
- Qt chia list thành nhiều phần nhỏ
- Mỗi phần chạy trên một thread trong thread pool
- Function lambda được gọi cho từng phần tử
- Thread chính bị block cho tới khi tất cả xong
