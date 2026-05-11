Khi ta đọc tài liệu thì sẽ thấy một component sẽ có các mục lớn như
- Method
- Properties
- Signals


- Với Method là những funciton xử lí từng chức năng cụ thể của component

- Signals thì khi xử ra sẽ có slot nhận signal đó và xử lí

Cú pháp của slot nhận signal: on + tên signal viết hoa chữ cái đầu

Vd: MouseArea có signal clicked -> slot: onClicked

- Properties thì khi giá trị thay đổi cũng sẽ phát ra signal

Vd: enabled -> phát ra signal: enabledChanged -> slot nhận xử lí: onEnabledChanged


Vd: Khi ta click chuột thì sẽ thay đổi giá trị của `visible` của Item => phát ra signal `visibleChanged`
~~~
    Item {
        id: myItem
        visible:  true

        Rectangle {
            width: 50
            height: 25
            color: "red"
        }

        // Xử lí
        onVisibleChanged: {
            if(visible) {
                console.log("Phan tu vua hien len")
            } else {
                console.log("Phan tu vua bi an đi")
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
             myItem.visible = !myItem.visible
        }
    }
    ~~~