import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: logoutBtnRoot
    width: 100
    height: 36

    // Đổi màu nền khi di chuột vào (hover)
    color: "transparent"
    radius: 6
    border.color: mouseArea.containsMouse ? "#EF4444" : "#1A2535"
    border.width: 1

    signal logoutClicked()

    RowLayout {
        anchors.centerIn: parent
        spacing: 8

        Text {
            text: "Log Out"
            color: "#475569"
            font.pixelSize: 13
            font.bold: true
            font.letterSpacing: 0.5
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: logoutBtnRoot.logoutClicked()
    }
}
