// VaultContextMenu.qml
import QtQuick 2.15
import QtQuick.Controls 2.15

Menu {
    id: rootMenu
    parent: Overlay.overlay
    property bool hasSelectedVault: false

    signal createRequested()
    signal renameRequested()
    signal deleteRequested()

    onClosed: rootMenu.destroy()

    background: Rectangle {
        color: "#1e293b"
        radius: 8
        border.color: "#334155"
        border.width: 1
    }

    MenuItem {
        text: "Create New Vault"
        height: 38
        contentItem: Text { text: parent.text; color: "white"; font.pixelSize: 13; verticalAlignment: Text.AlignVCenter }
        onTriggered: rootMenu.createRequested()
    }

    MenuItem {
        text: "Rename Vault"
        enabled: rootMenu.hasSelectedVault // SỬA: Đổi visible thành enabled
        height: 38
        contentItem: Text {
            text: parent.text;
            color: parent.enabled ? "white" : "#64748b"; // Thêm màu xám khi không bấm được
            font.pixelSize: 13; verticalAlignment: Text.AlignVCenter
        }
        onTriggered: rootMenu.renameRequested()
    }

    MenuItem {
        text: "Delete Vault"
        enabled: rootMenu.hasSelectedVault // SỬA: Đổi visible thành enabled
        height: 38
        contentItem: Text {
            text: parent.text;
            color: parent.enabled ? "#ef4444" : "#64748b"; // Thêm màu xám khi không bấm được
            font.pixelSize: 13; font.bold: true; verticalAlignment: Text.AlignVCenter
        }
        onTriggered: rootMenu.deleteRequested()
    }
}
