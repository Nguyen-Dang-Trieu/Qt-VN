import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15
import Qt.labs.platform 1.1

Popup {
    id: importPopup
    width: 460
    height: 400
    modal: true
    focus: true
    anchors.centerIn: Overlay.overlay

    background: Item {
        DropShadow {
            anchors.fill: bgRect
            horizontalOffset: 0; verticalOffset: 6
            radius: 12; samples: 24
            color: "#66000000"; source: bgRect
        }
        Rectangle {
            id: bgRect; anchors.fill: parent
            color: "#0f172a"; radius: 12
            border.color: "#1e293b"; border.width: 1
        }
    }

    signal vaultImported(string folderPath, string password)

    property bool isError: false
    property bool isSuccess: false

    function showError(msg) {
        statusMsg.text = msg
        statusMsg.color = "#ef4444"
        isError = true; isSuccess = false
        statusMsg.visible = true
    }

    function showSuccess(msg) {
        statusMsg.text = msg
        statusMsg.color = "#34D399"
        isError = false; isSuccess = true
        statusMsg.visible = true
        // Khóa các input khi thành công
        importBtn.enabled = false
        passwordInput.enabled = false
        browseBtn.enabled = false
    }

    ColumnLayout {
        anchors.fill: parent; anchors.margins: 30; spacing: 20

        ColumnLayout {
            Layout.fillWidth: true; spacing: 6
            Text {
                text: "Import Existing Vault"; color: "white"
                font.pixelSize: 22; font.bold: true
                horizontalAlignment: Text.AlignHCenter; Layout.fillWidth: true
            }
            Text {
                text: "Select your vault folder and enter its password."; color: "#94a3b8"
                font.pixelSize: 13; horizontalAlignment: Text.AlignHCenter; Layout.fillWidth: true
            }
        }

        ColumnLayout {
            Layout.fillWidth: true; spacing: 12

            // Folder Path
            ColumnLayout {
                spacing: 8; Layout.fillWidth: true
                Text { text: "VAULT FOLDER PATH"; color: "#8E9AAB"; font.pixelSize: 11; font.bold: true }
                RowLayout {
                    spacing: 10
                    TextField {
                        id: folderPathInput; Layout.fillWidth: true; Layout.preferredHeight: 44
                        placeholderText: "Select vault folder..."; color: "white"
                        readOnly: true; leftPadding: 12
                        background: Rectangle { color: "#1e293b"; radius: 8; border.color: "#334155" }
                    }
                    Button {
                        id: browseBtn
                        Layout.preferredHeight: 44
                        Layout.preferredWidth: 90

                        contentItem: Text {
                            text: "Browse"
                            color: "white"
                            font.bold: true
                            font.pixelSize: 13
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        background: Rectangle {
                            color: browseBtn.down ? "#0369a1" : "#0284c7"
                            radius: 8
                        }
                        onClicked: folderDialog.open()
                    }
                }
            }

            // Password
            ColumnLayout {
                spacing: 8; Layout.fillWidth: true
                Text { text: "PASSWORD"; color: "#8E9AAB"; font.pixelSize: 11; font.bold: true }
                TextField {
                    id: passwordInput; Layout.fillWidth: true; Layout.preferredHeight: 44
                    enabled: isFolderValid
                    echoMode: TextInput.Password; color: "white"; leftPadding: 12
                    background: Rectangle {
                        color: "#1e293b"; radius: 8
                        border.color: isSuccess ? "#34D399" : (isError ? "#ef4444" : "#334155")
                        border.width: (isError || isSuccess) ? 2 : 1
                    }
                    onTextChanged: if(isError) statusMsg.visible = false
                }
            }

            Text { id: statusMsg; font.pixelSize: 12; visible: false }
        }

        RowLayout {
            Layout.fillWidth: true; spacing: 15
            Button {
                id: cancelBtn
                Layout.fillWidth: true
                Layout.preferredHeight: 44

                contentItem: Text {
                    text: "Cancel"
                    color: "#94a3b8"
                    font.bold: true
                    font.pixelSize: 14
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle {
                    color: cancelBtn.hovered ? "#334155" : "transparent"
                    border.color: "#334155"
                    border.width: 1
                    radius: 8
                    opacity: cancelBtn.pressed ? 0.8 : 1.0
                }
                onClicked: importPopup.close()
            }
            Button {
                id: importBtn
                Layout.fillWidth: true
                Layout.preferredHeight: 44
                enabled: isFolderValid && folderPathInput.text !== "" && passwordInput.text !== ""

                contentItem: Text {
                    text: "Import Vault"
                    color: parent.enabled ? "white" : "#94a3b8"
                    font.bold: true
                    font.pixelSize: 14
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle {
                    color: {
                        if (isSuccess) return "#059669";
                        if (!importBtn.enabled) return "#1e293b";
                        if (importBtn.pressed) return "#0369a1";
                        if (importBtn.hovered) return "#0ea5e9";
                        return "#0284c7";
                    }
                    radius: 8

                    DropShadow {
                        anchors.fill: parent
                        horizontalOffset: 0
                        verticalOffset: 2
                        radius: 4
                        samples: 8
                        color: importBtn.enabled ? (isSuccess ? "#4d059669" : "#4d0284c7") : "transparent"
                        source: parent
                    }
                }
                onClicked: importPopup.vaultImported(folderPathInput.text, passwordInput.text)
            }
        }
    }

    FolderDialog {
        id: folderDialog
        onAccepted: {
            let path = folderDialog.folder.toString().replace(/^(file:\/{2,3})/, "")
            if (path[0] !== '/' && Qt.platform.os !== "windows") path = '/' + path
            folderPathInput.text = decodeURIComponent(path)

            let isValid = vaultController.checkVaultStructure(folderPathInput.text)

            if (isValid) {
                isFolderValid = true
                showSuccess("Vault structure verified. Please enter password.")
                passwordInput.forceActiveFocus() // Tự động focus để người dùng nhập pass
            } else {
                isFolderValid = false
                passwordInput.text = "" // Xóa pass cũ nếu có
                showError("Invalid Vault! Missing .db file or 'vault' folder.")
            }
        }
    }
}
