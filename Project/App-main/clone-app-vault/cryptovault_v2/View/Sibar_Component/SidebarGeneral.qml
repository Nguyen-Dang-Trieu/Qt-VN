import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ColumnLayout {
    property color primaryBlue
    property string activeMainMenu
    signal menuSelected(string menuName)

    Layout.fillWidth: true
    Layout.bottomMargin: 32
    spacing: 12

    Text { text: "GENERAL"; font.pixelSize: 11; color: "#475569"; font.bold: true; Layout.leftMargin: 4 }

    Repeater {
        model: [
            { n: "Overview", i: "📊" },
            { n: "Settings", i: "⚙️" }
        ]
        delegate: ItemDelegate {
            Layout.fillWidth: true
            height: 44
            readonly property bool isActive: activeMainMenu === modelData.n

            background: Rectangle {
                color: isActive ? "#1e293b" : (hovered ? "#161e2f" : "transparent")
                radius: 8
                Rectangle {
                    width: 4; height: 18; radius: 2
                    color: primaryBlue
                    anchors.left: parent.left; anchors.leftMargin: 2
                    anchors.verticalCenter: parent.verticalCenter
                    visible: isActive
                }
            }

            contentItem: RowLayout {
                spacing: 12; anchors.leftMargin: 12
                Text { text: modelData.i; font.pixelSize: 18; opacity: isActive ? 1.0 : 0.7 }
                Text {
                    text: modelData.n
                    color: isActive ? "#ffffff" : "#94a3b8"
                    font.bold: isActive; font.pixelSize: 14
                }
            }
            onClicked: menuSelected(modelData.n)
        }
    }
}
