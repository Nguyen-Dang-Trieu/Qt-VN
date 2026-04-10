import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Popup {
    id: createVaultPopup
    property color primaryBlue
    property color hoverBlue
    property color pressBlue
    property var vaultModel

    function openPopup() {
        newVaultName.text = ""
        newVaultPass.text = ""
        newVaultPass.showPassword = false
        createVaultPopup.open()
    }

    parent: Overlay.overlay
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    width: 300; modal: true; focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    background: Rectangle { color: "#1e293b"; radius: 12; border.color: "#334155"; border.width: 1 }

    ColumnLayout {
        anchors.fill: parent; anchors.margins: 10; spacing: 18

        Text { text: "Create New Vault"; color: "white"; font.bold: true; font.pixelSize: 18; Layout.bottomMargin: 5 }

        ColumnLayout {
            Layout.fillWidth: true; spacing: 8
            Text { text: "Vault Name"; color: "#94a3b8"; font.pixelSize: 12; font.bold: true }
            TextField {
                id: newVaultName
                Layout.fillWidth: true; height: 40; color: "white"
                placeholderText: "Enter name..."; placeholderTextColor: "#475569"
                leftPadding: 12; font.pixelSize: 14; selectByMouse: true
                background: Rectangle { color: "#0f172a"; radius: 8; border.color: parent.activeFocus ? primaryBlue : "#334155"; border.width: parent.activeFocus ? 2 : 1 }
                onAccepted: newVaultPass.forceActiveFocus()
            }
        }

        ColumnLayout {
            Layout.fillWidth: true; spacing: 8
            Text { text: "Vault Password"; color: "#94a3b8"; font.pixelSize: 12; font.bold: true }
            TextField {
                id: newVaultPass
                Layout.fillWidth: true; height: 40; color: "white"
                property bool showPassword: false
                echoMode: showPassword ? TextInput.Normal : TextInput.Password
                placeholderText: "••••••••"; placeholderTextColor: "#475569"
                leftPadding: 12; rightPadding: 40; font.pixelSize: 14; selectByMouse: true
                background: Rectangle { color: "#0f172a"; radius: 8; border.color: parent.activeFocus ? primaryBlue : "#334155"; border.width: parent.activeFocus ? 2 : 1 }

                Item {
                    anchors.right: parent.right; anchors.rightMargin: 12; anchors.verticalCenter: parent.verticalCenter
                    width: 20; height: 20
                    Text { anchors.centerIn: parent; text: "👁"; font.pixelSize: 16; color: newVaultPass.showPassword ? primaryBlue : "#64748b" }
                    Rectangle { anchors.centerIn: parent; width: 2; height: 18; color: "#64748b"; rotation: 45; visible: !newVaultPass.showPassword; antialiasing: true }
                    MouseArea { anchors.fill: parent; cursorShape: Qt.PointingHandCursor; onClicked: newVaultPass.showPassword = !newVaultPass.showPassword }
                                    }
                                    onAccepted: createBtn.clicked()
                                }
                            }

                            Button {
                                id: createBtn
                                Layout.fillWidth: true; Layout.preferredHeight: 42; Layout.topMargin: 10
                                text: "Confirm & Create"
                                contentItem: Text { text: parent.text; color: "white"; font.bold: true; font.pixelSize: 14; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
                                background: Rectangle { color: parent.down ? pressBlue : (parent.hovered ? hoverBlue : primaryBlue); radius: 8 }
                                onClicked: {
                                    let vName = newVaultName.text.trim()
                                    let vPass = newVaultPass.text

                                    if (vName !== "" && vPass !== "") {
                                        let success = vaultController.createVaultFolder(vName, vPass)

                                        if (success) {
                                            // ĐẢM BẢO LƯU PASSWORD VÀO MODEL
                                            vaultModel.append({
                                                                  name: vName,
                                                                  icon: "🔐",
                                                                  password: vPass // Lưu để dùng lại khi cần copy file
                                                              })
                                            createVaultPopup.close()
                                        }
                                    }
                                }
                            }
                        }
                    }
