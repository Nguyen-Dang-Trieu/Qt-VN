Hoạt động giống như một chồng các view, ta có thể đặt một view lên trên cùng để xem và có thể lấy nó ra để quay lại màn hình trước đó

# Properties
initialItem: View sẽ hiển thị cho user xem khi Stack view được khởi tạo.
currentItem: Lưu trữ phần tử hiện tại đang ở vị trí cao nhất trong stack
depth: số lượng view đang có trong stack
busy: true nếu stackview đang bận thực hiện hiệu ứng chuyển cảnh

## Transitions 
Định nghĩa các hiệu ứng khi chuyển đổi các view
- pushEnter/pushExit : Hiệu ứng khi add view mới
- popEnter/popExit: Hiệu ứng khi back view cũ

# Attached Properties
Thuộc tính đặc biệt

# Method 
Phương thức hành động điều khiển việc chuyển đổi màn hình
- push(Item): Thêm view mới đè lên màn hình hiện tại
- pop(): quay lại  view trước đó
- replace(Item): Thay thế view hiện tại bằng view mà ta muốn
- clear(): Xóa sạch các view


