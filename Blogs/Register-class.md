Để QML có thể nhìn thấy và gọi các hàm trong class, ta bắt buộc phải đăng kí class đó bên trong main.cpp

Các cách đăng kí phổ biến

## Cách 1: Đăng kí dưới dạng một Singleton
Cách này được sử dụng khi ta muốn một class dùng cho toàn bộ ứng dụng, và có thể gọi bất cứ đâu ở file.qml nào mà không cần phải khởi tạo lại
~~~cpp
// main.cpp
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "mycontroller.h" // File header của class bạn viết

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Đăng ký class MyController
    // Tham số: "com.mycompany.service" là tên module bạn tự đặt để import trong QML
    qmlRegisterSingletonInstance<MyController>("MyProject.Services", 1, 0, "MyController", new MyController);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    engine.load(url);

    return app.exec();
}
~~~

Trong QML, ta dùng như sau
~~~qml
import MyProject.Services 1.0

Button {
    onClicked: MyController.processData(...) 
}
~~~

## Cách 2: Đăng kí như một kiểu dữ liệu
Cách này dùng khi bạn muốn tạo ra nhiều đối tượng của class đó ngay trong QML.
~~~cpp
// main.cpp
qmlRegisterType<MyController>("MyProject.Types", 1, 0, "MyControllerType");
~~~

Trong QML
~~~
import MyProject.Types 1.0

MyControllerType {
    id: controller
}

Button {
    onClicked: controller.processData(...)
}
~~~

## Cách 3: Truyền thẳng vào ngữ cảnh
Đây là cách "mì ăn liền", đơn giản nhất nhưng hiện nay Qt không khuyến khích dùng cho các dự án lớn

~~~
// main.cpp
MyController controller;
engine.rootContext()->setContextProperty("myController", &controller);
~~~

Trong QML, không cần import gì cả dùng trực tiếp tên `myController`.