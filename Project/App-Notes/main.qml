import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 400
    height: 500
    title: "Notes MVC App"

    Column {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 10

        // Khung de nhap du lieu text
        TextField {
            id: input
            placeholderText: "Enter note..."
            onAccepted: addBtn.clicked()
        }

        // Button: Add note - Thêm dữ liệu vào trong database
        Button {
            id: addBtn
            text: "Add Note"

            onClicked: {
                notesController.addNote(input.text)
                input.text = ""
            }
        }

        ListView {
            width: parent.width
            height: 350
            model: notesModel // Nơi lấy dữ liệu của ListView

            delegate: Rectangle {
                width: parent.width
                height: 40
                color: "#f1f5f9"
                radius: 6

                Row {
                    anchors.fill: parent
                    anchors.margins: 8
                    spacing: 10

                    Text {
                        text: title
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    // Button: Delete - Xóa dữ liệu trong db
                    Button {
                        text: "Delete"
                        anchors.verticalCenter: parent.verticalCenter

                        // Với mỗi Item, QML tự tạo context
                        // index  = vị trí của item trong model (0, 1, 2, ...)
                        onClicked: notesController.removeNote(index)
                    }
                }
            }
        }
    }
}
