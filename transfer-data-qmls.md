Để tách MainContent.qml ra cho dễ maintain và mở rộng mà không làm thay đổi UI hay logic hiện tại, cách tốt nhất là áp dụng nguyên tắc Component-based. Chúng ta sẽ tách các cụm UI độc lập ra thành các file .qml riêng biệt và giao tiếp với file chính thông qua `Properties (truyền dữ liệu vào)` và `Signals (nhận sự kiện ra)`.

## 1. Nguyên tắc vàng 

Trong QML:
- Data đi xuống (parent → child) bằng property
- Event đi lên (child → parent) bằng signal

👉 Đây là pattern chuẩn (giống React / Flutter)

## 2. Parent → Child (truyền dữ liệu xuống)
-> Dùng property

- Child.qml
~~~
Rectangle {
    width: 200; height: 50
    color: "lightblue"

    property string message

    Text {
        anchors.centerIn: parent
        text: message
    }
}
~~~
- Main.qml (parent)
~~~
Column {
    Child {
        message: "Hello QML 👋"
    }
}
~~~
Kết quả 👉 Child hiển thị "Hello QML 👋"

### Bind là gì
Trong QML:
~~~
message: someVariable
~~~
- 👉 KHÔNG phải gán giá trị một lần
- 👉 Mà là ràng buộc (binding)

Nghĩ như này: message luôn luôn = someVariable

#### 🔍 Ví dụ dễ hiểu
- Main.qml
~~~
Column {

    property string myText: "Hello"

    Child {
        message: myText   // 👈 binding ở đây
    }

    Button {
        text: "Change text"
        onClicked: {
            myText = "Changed!"
        }
    }
}
~~~
- Child.qml
~~~
Text {
    property string message
    text: message
}
~~~

✅ Kết quả:

- Ban đầu:
> Child hiển thị: Hello

- Click button:
> myText = "Changed!" → Child tự update: Changed!

### Vì sao tự update?

Vì:
~~~
message: myText
~~~
👉 QML hiểu là:
~~~
message luôn phụ thuộc vào myText
~~~
→ Khi myText đổi → message tự đổi

## 3. Child → Parent (gửi sự kiện lên)
Ví dụ click button

- Child.qml
~~~
Rectangle {
    width: 200; height: 50
    color: "lightgreen"

    signal clicked() // định nghĩa signal

    MouseArea {
        anchors.fill: parent
        onClicked: { 
            clicked()   // báo lên parent
        }
    }

    Text {
        anchors.centerIn: parent
        text: "Click me"
    }
}
~~~

- Main.qml (Parent)
~~~
Column {
    Child {
        onClicked: { // Handler xử lí signal
            console.log("Child was clicked!")
        }
    }
}
~~~
✅ Kết quả 👉 Click vào Child → Parent nhận được event

## 4. Kết hợp 2 chiều
🎯 Ví dụ thực tế 

👉 Click button → đổi text

- Child.qml
~~~
Rectangle {
    width: 200; height: 50
    color: "lightgreen"

    property string label
    signal clicked()

    MouseArea {
        anchors.fill: parent
        onClicked: clicked()
    }

    Text {
        anchors.centerIn: parent
        text: label
    }
}
~~~

- Main.qml
~~~
Column {

    property string currentText: "Click me"

    Child {
        label: currentText

        onClicked: {
            currentText = "You clicked!"
        }
    }
}
~~~

Flow xảy ra
~~~
Parent → Child : label = currentText
Child → Parent : clicked()
Parent xử lý → đổi currentText
→ Child tự update UI
~~~

## 5. 5. Sibling

❌ Sai
~~~
ChildA gọi ChildB trực tiếp
~~~

✅ Đúng

- Main.qml
~~~
Column {

    property string sharedText: "Hello"

    ChildA {
        onClicked: {
            sharedText = "Changed by A"
        }
    }

    ChildB {
        textValue: sharedText
    }
}
~~~

- ChildA.qml
~~~
Rectangle {
    width: 100; height: 50
    color: "orange"

    signal clicked()

    MouseArea {
        anchors.fill: parent
        onClicked: clicked()
    }

    Text { anchors.centerIn: parent; text: "A" }
}
~~~

- ChildB.qml
~~~
Rectangle {
    width: 200; height: 50
    color: "lightblue"

    property string textValue

    Text {
        anchors.centerIn: parent
        text: textValue
    }
}
~~~

Flow
~~~
ChildA → signal → Parent → update data → ChildB nhận data
~~~

# Tóm lại
~~~
Parent → Child : property       (truyền dữ liệu)
Child → Parent : signal          (báo sự kiện)
Sibling        : qua parent
~~~