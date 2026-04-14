// SideBar/Popup/CreateVaultPopup.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.1

Popup {
    id: root

    // Properties truyền từ Sidebar để đồng bộ màu sắc [cite: 13-16]
    property color primaryBlue: "#246a99"
    property color hoverBlue: "#2d82bc"
    property color pressBlue: "#1b5175"

    // Signal gửi dữ liệu ngược lại cho Sidebar xử lý C++ [cite: 12, 33]
    signal createConfirmed(string name, string password, string path)

    parent: Overlay.overlay
    x: Math.round((parent.width - width) / 2)
    y: Math.round((parent.height - height) / 2)
    width: 300
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    background: Rectangle {
        color: "#1e293b"
        radius: 12
        border.color: "#334155"
        border.width: 1
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 18

        Text {
            text: "Create New Vault"
            color: "white"; font.bold: true; font.pixelSize: 18
            Layout.bottomMargin: 20
        }

        // --- Trường nhập tên Vault [cite: 86-93] ---
        ColumnLayout {
            Layout.fillWidth: true; spacing: 8
            Text { text: "Vault Name"; color: "#94a3b8"; font.pixelSize: 12; font.bold: true }
            TextField {
                id: newVaultName
                Layout.fillWidth: true; height: 40; color: "white"
                placeholderText: "Enter name..."; font.pixelSize: 14; selectByMouse: true
                background: Rectangle {
                    color: "#0f172a"; radius: 8
                    border.color: parent.activeFocus ? root.primaryBlue : "#334155"
                    border.width: parent.activeFocus ? 2 : 1
                }
                onAccepted: newVaultPass.forceActiveFocus()
            }
        }

        // --- Trường nhập Password [cite: 94-112] ---
        ColumnLayout {
            Layout.fillWidth: true; spacing: 8
            Text { text: "Vault Password"; color: "#94a3b8"; font.pixelSize: 12; font.bold: true }
            TextField {
                id: newVaultPass
                Layout.fillWidth: true; height: 40; color: "white"
                property bool showPassword: false
                echoMode: showPassword ? TextInput.Normal : TextInput.Password
                placeholderText: "••••••••"; selectByMouse: true
                background: Rectangle {
                    color: "#0f172a"; radius: 8
                    border.color: parent.activeFocus ? root.primaryBlue : "#334155"
                    border.width: parent.activeFocus ? 2 : 1
                }
                // Nút ẩn/hiện mật khẩu
                Item {
                    anchors.right: parent.right
                    anchors.rightMargin: 12
                    anchors.verticalCenter: parent.verticalCenter
                    width: 20
                    height: 20

                    Image {
                        anchors.fill: parent
                        source: newVaultPass.showPassword ? "qrc:/View/Icons/visibility.svg" : "qrc:/View/Icons/visibility_off.svg"
                        fillMode: Image.PreserveAspectFit
                        mipmap: true // Giúp SVG hiển thị mượt mà hơn khi thu nhỏ
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: newVaultPass.showPassword = !newVaultPass.showPassword
                    }
                }
            }
        }

        // --- Trường chọn đường dẫn [cite: 113-127] ---
        ColumnLayout {
            Layout.fillWidth: true; spacing: 8
            Text { text: "Storage Path"; color: "#94a3b8"; font.pixelSize: 12; font.bold: true }
            RowLayout {
                Layout.fillWidth: true; spacing: 8
                TextField {
                    id: newVaultPath
                    Layout.fillWidth: true; height: 40; color: "white"
                    placeholderText: "e.g. /home/MyVaults"; selectByMouse: true
                    background: Rectangle {
                        color: "#0f172a"; radius: 8
                        border.color: parent.activeFocus ? root.primaryBlue : "#334155"
                        border.width: parent.activeFocus ? 2 : 1
                    }
                }
                Rectangle {
                    Layout.preferredWidth: 40; Layout.preferredHeight: 40
                    color: pathMouseArea.containsMouse ? root.hoverBlue : "#334155"; radius: 8
                    Text { text: "📁"; anchors.centerIn: parent; font.pixelSize: 16 }
                    MouseArea { id: pathMouseArea; anchors.fill: parent; cursorShape: Qt.PointingHandCursor; hoverEnabled: true; onClicked: folderDialog.open() }
                }
            }
        }

        // --- Nút xác nhận [cite: 128-131] ---
        Button {
            Layout.fillWidth: true; Layout.preferredHeight: 42; Layout.topMargin: 10; text: "Confirm & Create"
            contentItem: Text { text: parent.text; color: "white"; font.bold: true; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
            background: Rectangle { color: parent.down ? root.pressBlue : (parent.hovered ? root.hoverBlue : root.primaryBlue); radius: 8 }
            onClicked: {
                if (newVaultName.text !== "" && newVaultPass.text !== "" && newVaultPath.text !== "") {
                    root.createConfirmed(newVaultName.text.trim(), newVaultPass.text, newVaultPath.text.trim())
                }
            }
        }
    }

    FolderDialog {
        id: folderDialog
        title: "Select Vault Storage Folder"
        onAccepted: {
            let path = folder.toString()
            if (Qt.platform.os === "windows") path = path.replace(/^(file:\/{3})/, "")
            else path = path.replace(/^(file:\/{2})/, "")
            newVaultPath.text = decodeURIComponent(path)
        }
    }
}
