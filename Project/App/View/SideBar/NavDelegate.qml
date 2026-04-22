// File: NavDelegate.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ItemDelegate {
    id: delegateRoot
    Layout.fillWidth: true
    height: 40

    // --- 1. NHẬN DỮ LIỆU TỪ COMPONENT CHA ---
    property int selectedIndex: -1
    property color primaryColor: "#0284c7"

    // Tính toán xem item này có đang được chọn không (dựa vào index của Repeater)
    readonly property bool isCurrent: delegateRoot.selectedIndex === index

    // --- 2. KHAI BÁO TÍN HIỆU ---
    signal vaultClicked(int clickedIndex)
    signal vaultDoubleClicked(int clickedIndex)
    signal changeVaultPasswordRequested(int clickedIndex)

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
        anchors.leftMargin: 12

        Text {
            text: model.icon // Lấy từ vaultModel
            font.pixelSize: 14
        }
        Text {
            text: model.name // Lấy từ vaultModel
            color: isCurrent ? "#ffffff" : "#94a3b8"
            font.bold: isCurrent
            font.pixelSize: 13

            Layout.fillWidth: true
            elide: Text.ElideRight

        }
    }

    // --- 4. XỬ LÝ SỰ KIỆN ---
    onClicked: {
        // Phát tín hiệu lên file cha kèm theo vị trí (index) vừa click
        delegateRoot.vaultClicked(index)
    }

    onDoubleClicked: delegateRoot.vaultDoubleClicked(index)

    // --- 5. BẮT SỰ KIỆN CHUỘT PHẢI & MENU ---
    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: {
            delegateRoot.vaultClicked(index)
            // 1. Quy đổi tọa độ chuột từ Delegate sang hệ tọa độ của SideBar
            let mappedPos = delegateRoot.mapToItem(sidebarRoot, mouseX, mouseY)

            // 2. Truyền tọa độ đã quy đổi cho hàm hiện Menu
            sidebarRoot.showVaultContextMenu(mappedPos.x, mappedPos.y, index)
        }
    }
}
