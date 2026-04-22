import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

Popup {
    id: changePassVaultPopup
    width: 450
    // Height tự động co giãn theo nội dung
    modal: true
    focus: true
    anchors.centerIn: Overlay.overlay
    // --- 1. LOGIC TRUYỀN DỮ LIỆU ---
    property string vaultName: "My Vault"
    signal changeVaultPasswordRequested(string currentPass, string newPass)

    property bool isError: false
    property bool isSuccess: false

    function showError(msg) {
        statusMsg.text = msg
        statusMsg.color = "#ef4444" // Đỏ
        isError = true
        isSuccess = false
        statusMsg.visible = true
    }

    function showSuccess(msg) {
        statusMsg.text = msg
        statusMsg.color = "#34D399" // Xanh Mint
        isError = false
        isSuccess = true
        statusMsg.visible = true

        // Khóa các ô nhập liệu khi thành công
        currentPassInput.enabled = false
        newPassInput.enabled = false
        confirmPassInput.enabled = false
        confirmBtn.enabled = false
    }
    // --- 2. NỀN & HIỆU ỨNG (Giống UnlockVaultPopup) ---
    background: Item {
        DropShadow {
            anchors.fill: bgRect
            horizontalOffset: 0
            verticalOffset: 6
            radius: 12
            samples: 24
            color: "#66000000"
            source: bgRect
        }

        Rectangle {
            id: bgRect
            anchors.fill: parent
            color: "#0f172a"
            radius: 12
            border.color: "#1e293b"
            border.width: 1
        }
    }



    // --- 3. BỐ CỤC CHÍNH ---
    contentItem: ColumnLayout {
        anchors.fill: parent
        anchors.margins: 30
        spacing: 20

        // --- TITLE ---
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 6
            Text {
                text: "Change Password"
                color: "white"
                font.pixelSize: 22
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
            }
            Text {
                text: "For vault: " + vaultName
                color: "#94a3b8"
                font.pixelSize: 13
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
            }
        }

        // Khoảng cách
        Item { Layout.preferredHeight: 5 }

        // --- Ô 1: CURRENT PASSWORD ---
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 8
            Text { text: "CURRENT PASSWORD"; color: "#8E9AAB"; font.pixelSize: 11; font.bold: true; font.letterSpacing: 1 }

            TextField {
                id: currentPassInput
                Layout.fillWidth: true
                Layout.preferredHeight: 44
                color: "white"
                font.pixelSize: 14
                leftPadding: 15; rightPadding: 45
                placeholderText: "Enter current password"
                property bool showPass: false
                echoMode: showPass ? TextInput.Normal : TextInput.Password

                background: Rectangle {
                    color: "#1e293b"; radius: 8
                    border.color: parent.activeFocus ? "#0284c7" : "#334155"
                }

                Item {
                    anchors.right: parent.right; anchors.rightMargin: 12; anchors.verticalCenter: parent.verticalCenter
                    width: 18; height: 18
                    Image {
                        anchors.fill: parent
                        source: currentPassInput.showPass ? "qrc:/View/Icons/visibility.svg" : "qrc:/View/Icons/visibility_off.svg"
                        mipmap: true
                    }
                    MouseArea {
                        anchors.fill: parent; cursorShape: Qt.PointingHandCursor
                        onClicked: currentPassInput.showPass = !currentPassInput.showPass
                    }
                }
                onTextChanged: if (isError) { statusMsg.visible = false; isError = false }
            }
        }

        // --- Ô 2: NEW PASSWORD ---
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 8
            Text { text: "NEW PASSWORD"; color: "#8E9AAB"; font.pixelSize: 11; font.bold: true; font.letterSpacing: 1 }

            TextField {
                id: newPassInput
                Layout.fillWidth: true
                Layout.preferredHeight: 44
                color: "white"
                font.pixelSize: 14
                leftPadding: 15; rightPadding: 45
                placeholderText: "Enter new password"
                property bool showPass: false
                echoMode: showPass ? TextInput.Normal : TextInput.Password

                background: Rectangle {
                    color: "#1e293b"; radius: 8
                    border.color: parent.activeFocus ? "#0284c7" : "#334155"
                }

                Item {
                    anchors.right: parent.right; anchors.rightMargin: 12; anchors.verticalCenter: parent.verticalCenter
                    width: 18; height: 18
                    Image {
                        anchors.fill: parent
                        source: newPassInput.showPass ? "qrc:/View/Icons/visibility.svg" : "qrc:/View/Icons/visibility_off.svg"
                        mipmap: true
                    }
                    MouseArea {
                        anchors.fill: parent; cursorShape: Qt.PointingHandCursor
                        onClicked: newPassInput.showPass = !newPassInput.showPass
                    }
                }
                onTextChanged: if (isError) { statusMsg.visible = false; isError = false }
            }
        }

        // --- Ô 3: CONFIRM NEW PASSWORD ---
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 8
            Text { text: "CONFIRM NEW PASSWORD"; color: "#8E9AAB"; font.pixelSize: 11; font.bold: true; font.letterSpacing: 1 }

            TextField {
                id: confirmPassInput
                Layout.fillWidth: true
                Layout.preferredHeight: 44
                color: "white"
                font.pixelSize: 14
                leftPadding: 15; rightPadding: 45
                placeholderText: "Re-enter new password"
                property bool showPass: false
                echoMode: showPass ? TextInput.Normal : TextInput.Password

                // Logic kiểm tra trùng khớp
                property bool isMatch: text.length > 0 && text === newPassInput.text

                background: Rectangle {
                    color: "#1e293b"; radius: 8
                    border.color: {
                        if (confirmPassInput.isMatch) return "#34D399"; // Xanh nếu khớp
                        if (confirmPassInput.text.length > 0 && !confirmPassInput.isMatch) return "#ef4444"; // Đỏ nếu sai
                        if (confirmPassInput.activeFocus) return "#0284c7";
                        return "#334155";
                    }
                }

                Item {
                    anchors.right: parent.right; anchors.rightMargin: 12; anchors.verticalCenter: parent.verticalCenter
                    width: 18; height: 18
                    Image {
                        anchors.fill: parent
                        source: confirmPassInput.showPass ? "qrc:/View/Icons/visibility.svg" : "qrc:/View/Icons/visibility_off.svg"
                        mipmap: true
                    }
                    MouseArea {
                        anchors.fill: parent; cursorShape: Qt.PointingHandCursor
                        onClicked: confirmPassInput.showPass = !confirmPassInput.showPass
                    }
                }
                onTextChanged: if (isError) { statusMsg.visible = false; isError = false }
            }

            // Dòng text báo lỗi không khớp pass
            Text {
                text: "Passwords do not match!"
                color: "#ef4444"
                font.pixelSize: 11
                visible: confirmPassInput.text.length > 0 && !confirmPassInput.isMatch
            }
        }

        // --- STATUS MESSAGE (Dùng chung cho báo lỗi C++ hoặc Thành công) ---
        Text {
            id: statusMsg
            text: ""
            font.pixelSize: 12
            visible: false
            Layout.alignment: Qt.AlignHCenter
        }

        // --- 4. ACTION BUTTONS ---
        RowLayout {
            Layout.fillWidth: true
            Layout.bottomMargin: 20
            spacing: 15

            // Cancel Button
            Button {
                Layout.fillWidth: true
                Layout.preferredHeight: 44
                contentItem: Text {
                    text: "Cancel"
                    color: "#94a3b8"
                    font.bold: true; font.pixelSize: 14
                    horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
                }
                background: Rectangle {
                    color: parent.hovered ? "#334155" : "transparent"
                    border.color: "#334155"; border.width: 1; radius: 8
                }
                onClicked: changePassVaultPopup.close()
            }

            // Confirm Button
            Button {
                id: confirmBtn
                Layout.fillWidth: true
                Layout.preferredHeight: 44

                // Chỉ bật nút khi: Mật khẩu cũ không trống, Pass mới không trống và Pass mới khớp Pass xác nhận
                enabled: currentPassInput.text !== "" && newPassInput.text !== "" && confirmPassInput.isMatch

                contentItem: Text {
                    text: "Change Password"
                    color: parent.enabled ? "white" : "#94a3b8"
                    font.bold: true; font.pixelSize: 14
                    horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
                }
                background: Rectangle {
                    color: {
                        if (isSuccess) return "#059669";
                        if (!parent.enabled) return "#1e293b";
                        if (parent.pressed) return "#0369a1";
                        if (parent.hovered) return "#0ea5e9";
                        return "#0284c7";
                    }
                    radius: 8
                }
                onClicked: {
                    changePassVaultPopup.changeVaultPasswordRequested(currentPassInput.text, newPassInput.text)
                }
            }
        }
    }

    onOpened: currentPassInput.forceActiveFocus()
}
