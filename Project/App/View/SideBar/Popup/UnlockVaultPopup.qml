import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15 // Thêm thư viện này để tạo hiệu ứng bóng đổ

Popup {
    id: passwordPopup
    width: 420
    height: 300
    modal: true
    focus: true
    anchors.centerIn: Overlay.overlay

    // --- 1. NỀN & HIỆU ỨNG ---
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

    // --- 2. LOGIC TRUYỀN DỮ LIỆU ---
    property string vaultName: ""
    signal unlockRequested(string password)

    // Khai báo trạng thái để đổi màu UI
    property bool isError: false
    property bool isSuccess: false

    function showError(msg) {
        statusMsg.text = msg
        statusMsg.color = "#ef4444" // Báo màu Đỏ
        isError = true
        isSuccess = false
        statusMsg.visible = true
    }

    function showSuccess(msg) {
        statusMsg.text = msg
        statusMsg.color = "#34D399" // Báo màu Xanh Mint
        isError = false
        isSuccess = true
        statusMsg.visible = true

        // Khóa các nút bấm để chống Spam Click khi đang chờ
        passwordInput.enabled = false
        unlockBtn.enabled = false
        cancelBtn.enabled = false
    }

    // --- 3. BỐ CỤC CHÍNH ---
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 30
        spacing: 24

        // --- TITLE & DESCRIPTION ---
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 6

            Text {
                text: "Unlock '" + vaultName + "' Vault"
                color: "white"
                font.pixelSize: 22
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
            }
        }

        // --- 4. KHỐI NHẬP LIỆU ---
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 12

            Text {
                text: "YOUR VAULT PASSWORD"
                color: passwordInput.activeFocus ? "#0284c7" : "#8E9AAB"
                font.pixelSize: 11
                font.bold: true
                font.letterSpacing: 1
                Layout.leftMargin: 4
            }

            TextField {
                id: passwordInput
                Layout.fillWidth: true
                Layout.preferredHeight: 44
                placeholderText: "Enter password"
                echoMode: TextInput.Password
                color: "white"
                font.pixelSize: 14
                leftPadding: 12
                rightPadding: 12

                background: Rectangle {
                    id: inputBg
                    color: "#1e293b"
                    radius: 8

                    // Đổi màu viền dựa theo trạng thái
                    border.color: {
                        if (isSuccess) return "#34D399"; // Xanh thành công
                        if (isError) return "#ef4444";   // Đỏ thất bại
                        if (passwordInput.activeFocus) return "#0284c7";
                        return "#334155";
                    }
                    border.width: (passwordInput.activeFocus || isError || isSuccess) ? 2 : 1
                }

                Keys.onReturnPressed: {
                    if (text !== "" && !isSuccess) passwordPopup.unlockRequested(text)
                }

                onTextChanged: {
                    if (isError) {
                        statusMsg.visible = false
                        isError = false
                    }
                }
            }

            // Text Thông báo dùng chung cho cả Lỗi và Thành công
            Text {
                id: statusMsg
                text: ""
                font.pixelSize: 12
                visible: false
                Layout.leftMargin: 4
            }
        }

        Item { Layout.fillHeight: true }

        // --- 5. NÚT HÀNH ĐỘNG ---
        RowLayout {
            Layout.fillWidth: true
            spacing: 15

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
                onClicked: passwordPopup.close()
            }

            Button {
                id: unlockBtn
                Layout.fillWidth: true
                Layout.preferredHeight: 44
                enabled: passwordInput.text !== ""

                contentItem: Text {
                    text: "Unlock Vault"
                    color: parent.enabled ? "white" : "#94a3b8"
                    font.bold: true
                    font.pixelSize: 14
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle {
                    color: {
                        if (isSuccess) return "#059669"; // Xanh Emerald đậm khi thành công
                        if (!unlockBtn.enabled) return "#1e293b";
                        if (unlockBtn.pressed) return "#0369a1";
                        if (unlockBtn.hovered) return "#0ea5e9";
                        return "#0284c7";
                    }
                    radius: 8

                    DropShadow {
                        anchors.fill: parent
                        horizontalOffset: 0
                        verticalOffset: 2
                        radius: 4
                        samples: 8
                        color: unlockBtn.enabled ? (isSuccess ? "#4d059669" : "#4d0284c7") : "transparent"
                        source: parent
                    }
                }
                onClicked: passwordPopup.unlockRequested(passwordInput.text)
            }
        }
    }

    onOpened: passwordInput.forceActiveFocus()
}
