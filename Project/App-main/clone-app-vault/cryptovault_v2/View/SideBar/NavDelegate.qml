// File: NavDelegate.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ItemDelegate {
    id: delegateRoot
    Layout.fillWidth: true
    width: parent.width
    height: 40

    // --- 1. NHẬN DỮ LIỆU TỪ COMPONENT CHA ---
    property int selectedIndex: -1
    property color primaryColor: "#0284c7"

    // Tính toán xem item này có đang được chọn không (dựa vào index của Repeater)
    readonly property bool isCurrent: delegateRoot.selectedIndex === index

    // --- 2. KHAI BÁO TÍN HIỆU ---
    signal vaultClicked(int clickedIndex)

    // --- 3. GIAO DIỆN ---
    background: Rectangle {
        color: isCurrent ? "#1e293b" : (delegateRoot.hovered ? "#161e2f" : "transparent")
        radius: 8
        Rectangle {
            width: 1
            height: 16
            radius: 2
            color: delegateRoot.primaryColor
            anchors.left: parent.left
            anchors.leftMargin: 2
            anchors.verticalCenter: parent.verticalCenter
            visible: isCurrent
        }
    }

    contentItem: RowLayout {
        spacing: 12
        anchors.leftMargin: 24
        Text {
            text: model.icon // Lấy từ vaultModel
            font.pixelSize: 14
        }
        Text {
            text: model.name // Lấy từ vaultModel
            color: isCurrent ? "#ffffff" : "#94a3b8"
            font.bold: isCurrent
            font.pixelSize: 13
        }
    }

    // --- 4. XỬ LÝ SỰ KIỆN ---
    onClicked: {
        // Phát tín hiệu lên file cha kèm theo vị trí (index) vừa click
        delegateRoot.vaultClicked(index)
    }
}
