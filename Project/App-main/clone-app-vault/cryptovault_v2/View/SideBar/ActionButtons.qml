import QtQuick 2.15
import QtQuick.Layouts 1.15

ColumnLayout {
    id: actionRoot
    spacing: 8

    // --- 1. NHẬN DỮ LIỆU TỪ COMPONENT CHA ---
    property color primaryColor: "#0284c7" // Default color
    property int selectedIndex: -1

    // --- 2. KHAI BÁO CÁC TÍN HIỆU (SIGNALS) ---
    signal createClicked()
    signal renameClicked()
    signal deleteClicked()

    // --- 3. GIAO DIỆN CÁC NÚT ---
    // Nút "Create New Vault"
    Rectangle {
        Layout.fillWidth: true
        height: 38
        color: actionRoot.primaryColor
        radius: 8
        Text {
            anchors.centerIn: parent
            text: "+ Create New Vault"
            color: "white"
            font.pixelSize: 12
            font.bold: true
        }
        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: actionRoot.createClicked() // Phát tín hiệu
        }
    }

    // Hàng chứa nút "Rename" và "Delete"
    RowLayout {
        Layout.fillWidth: true
        spacing: 8

        // Nút Rename
        Rectangle {
            Layout.fillWidth: true
            height: 36
            color: "#1e293b"
            radius: 6
            opacity: actionRoot.selectedIndex !== -1 ? 1.0 : 0.4

            Text {
                anchors.centerIn: parent
                text: "Rename"
                color: "#94a3b8"
                font.pixelSize: 11
                font.bold: true
            }
            MouseArea {
                anchors.fill: parent
                enabled: actionRoot.selectedIndex !== -1
                onClicked: actionRoot.renameClicked() // Phát tín hiệu
            }
        }

        // Nút Delete
        Rectangle {
            Layout.fillWidth: true
            height: 36
            color: "#450a0a"
            radius: 6
            border.color: "#ef4444"
            opacity: actionRoot.selectedIndex !== -1 ? 1.0 : 0.4

            Text {
                anchors.centerIn: parent
                text: "Delete"
                color: "#ef4444"
                font.pixelSize: 11
                font.bold: true
            }
            MouseArea {
                anchors.fill: parent
                enabled: actionRoot.selectedIndex !== -1
                onClicked: actionRoot.deleteClicked() // Phát tín hiệu
            }
        }
    }
}
