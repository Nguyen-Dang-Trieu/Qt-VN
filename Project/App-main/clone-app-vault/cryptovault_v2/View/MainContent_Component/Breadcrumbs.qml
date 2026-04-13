import QtQuick 2.15
import QtQuick.Layouts 1.15

/*
File này quản lí việc hiển thị đường dẫn và phát tín hiệu signal khi user click vào một thư mục cha

*/

RowLayout {
    id: root
    spacing: 5

    // Nhận data từ file chính (MainContent.qml)
    property var pathStack: []

    // Gửi sự kiện ra ngoài khi click
    signal folderClicked(int index, int targetId)

    Repeater {
        model: root.pathStack
        delegate: Row {
            spacing: 5
            Text {
                text: modelData.name
                color: index === root.pathStack.length - 1 ? "#38bdf8" : "#94a3b8"
                font.bold: true
                font.pixelSize: 14

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        if (index < root.pathStack.length - 1) {
                            root.folderClicked(index, modelData.id)
                        }
                    }
                }
            }
            Text {
                text: " > "
                color: "#475569"
                font.pixelSize: 14
                visible: index < root.pathStack.length - 1
            }
        }
    }
}
