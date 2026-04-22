// File: MenuPopup.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Popup {
    id: menuRoot
    width: 140
    padding: 4

    // Đảm bảo Popup tự động đóng khi click ra ngoài vùng trống
    closePolicy: Popup.CloseOnPressOutside | Popup.CloseOnEscape

    // Các thuộc tính nhận dữ liệu
    property int targetNodeId: -1
    property string targetNodeName: ""
    property int targetNodeType: -1

    // Tín hiệu
    signal exportRequested(int selectedId, string selectedName,int selectedType) // NEW
    signal deleteRequested(int selectedId, string selectedName)

    background: Rectangle {
        color: "#0D1321"
        border.color: "#1A2535"
        border.width: 1
        radius: 8
    }

    contentItem: ColumnLayout {
        spacing: 2

        // --- NÚT EXPORT ---
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 36
            radius: 6
            color: exportMouse.containsMouse ? "#121A2F" : "transparent"

            Text {
                anchors.fill: parent
                anchors.leftMargin: 10
                text: "Export"
                color: exportMouse.containsMouse ? "#4EA8FE" : "white"
                font.pixelSize: 13
                font.bold: true
                verticalAlignment: Text.AlignVCenter
            }

            MouseArea {
                id: exportMouse
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    menuRoot.exportRequested(menuRoot.targetNodeId, menuRoot.targetNodeName, menuRoot.targetNodeType)
                    menuRoot.close() // Tự đóng Popup sau khi chọn
                }
            }
        }

        // --- NÚT DELETE ---
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 36
            radius: 6
            color: deleteMouse.containsMouse ? "#2C1318" : "transparent"

            Text {
                anchors.fill: parent
                anchors.leftMargin: 10
                text: "Delete"
                color: deleteMouse.containsMouse ? "#EF4444" : "#8E9AAB"
                font.pixelSize: 13
                font.bold: true
                verticalAlignment: Text.AlignVCenter
            }

            MouseArea {
                id: deleteMouse
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    menuRoot.deleteRequested(menuRoot.targetNodeId, menuRoot.targetNodeName)
                    menuRoot.close() // Tự đóng Popup sau khi chọn
                }
            }
        }
    }
}
