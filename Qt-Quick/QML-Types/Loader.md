Loader la mot thanh phan cuc ky huu ich dung de tri hoan viec khoi tao cac item. Thay vi load toan bo giao dien ngay tu dau, Loader cho phep chi tai nhung gi
can thiet khi can den.

Tai sao nen dung Loader
- Toi uu hieu nang: Giam time khoi dong ung dung bang cach khong load cac thanh phan chua tung toi (vi du: cac tab an, cac hop thoai menu)
- Quan li bo nho: Co the giai phong bo nho bang cach dat source = "" hoac sourceComponent = undefined de huy item hien tai
- Giao dien dong: De dang thay doi noi dung hien thi dua tren logic code.

Co 2 cach su dung co ban cho Loader
- Su dung source (Load tu file .qml ben ngoai)
~~~
Loader {
  id: myLoader
  source: "MyButton.qml"
}
~~~
- Su dung sourceComponent (Load tu mot Component co san trong file)
~~~
Component {
  id: redRect
  Rectangle {color: "red"; width: 100; height: 100 }
}

Loader {
  id: pageLoaer
  sourceComponent: redRect
}
~~~

Tương tac voi Item duoc load
Khi Loader tai mot thanh phan, doi tuong do tro thanh item cua Loader. Co the truy cap cac thuoc tinh cua no thong qua tu khoa item

~~~
Loader {
  id: loader
  sourceComponent: Component {
     Rectangle {
        property string message: "Hello World"
     }
  }
  
  onLoaded: {
    console.log(loader.item.message)
  }
}
~~~
Truyen du lieu vao trong Loader dung thuoc tinh `setSource()`.


## Lưu ý
### 1. Sự khác biệt giữa "load" và "hiển thị" (visible)
- Trạng thái load: nó xảy ra lặp tức ngay khi úng dụng chạy. Nghĩa là component trong `sourceComponent` sẽ dược nạp vào memory.
- Trạng thái hiển thị: Phụ thuộc vào thuộc tính `visible` khi nào được set = true