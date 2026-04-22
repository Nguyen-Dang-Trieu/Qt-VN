import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

Popup {
    id: forgetPassPopup
    width: 480
    modal: true
    focus: true
    anchors.centerIn: Overlay.overlay

    // --- Properties ---
    property string vaultName: ""
    property bool isKekVerified: false // Trạng thái kiểm tra KEK

    // --- Signals ---
    signal verifyKekRequested(string kek)
    signal resetPasswordRequested(string kek, string newPass)

    background: Rectangle {
        color: "#0D1321"
        border.color: "#1A2535"
        border.width: 1
        radius: 8
    }

    contentItem: ColumnLayout {
        anchors.fill: parent
        anchors.margins: 25
        spacing: 20

        // --- Header ---
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 5
            Text {
                text: "Vault Recovery"
                color: "white"
                font.pixelSize: 22
                font.bold: true
                Layout.alignment: Qt.AlignHCenter
            }
            Text {
                text: "Use your Recovery Key to regain access to: " + vaultName
                color: "#8E9AAB"
                font.pixelSize: 13
                wrapMode: Text.Wrap
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
            }
        }

        // --- GIAI ĐOẠN 1: NHẬP KEK ---
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 8
            Text { text: "RECOVERY KEY"; color: "#8E9AAB"; font.pixelSize: 11; font.bold: true; font.letterSpacing: 1 }

            TextField {
                id: kekInput
                Layout.fillWidth: true
                Layout.preferredHeight: 44
                color: isKekVerified ? "#34D399" : "white"
                placeholderText: "Enter your 64-character KEY..."
                placeholderTextColor: "#5E6A7A"
                readOnly: isKekVerified // Khóa sau khi đã verify thành công
                selectByMouse: true
                selectionColor: "#4EA8FE"
                selectedTextColor: "#FFFFFF"
                background: Rectangle {
                    color: "#000000"
                    radius: 6
                    border.color: isKekVerified ? "#34D399" : (parent.activeFocus ? "#4EA8FE" : "#1A2535")
                    border.width: 1
                }
            }

            Button {
                id: verifyBtn
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                visible: !isKekVerified
                enabled: kekInput.text.length > 10

                contentItem: Text {
                    text: "Verify Recovery Key"
                    color: parent.enabled ? "black" : "#5E6A7A"
                    font.bold: true; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
                }
                background: Rectangle {
                    color: parent.enabled ? (parent.pressed ? "#3A80C9" : "#4EA8FE") : "#1A2535"
                    radius: 6
                }
                onClicked: verifyKekRequested(kekInput.text)
            }
        }

        // --- GIAI ĐOẠN 2: NHẬP MẬT KHẨU MỚI (CHỈ HIỆN KHI KEK OK) ---
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 15
            opacity: isKekVerified ? 1.0 : 0.3
            enabled: isKekVerified // Chỉ cho phép nhập khi KEK load thành công

            // Trường New Password
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 8
                Text { text: "NEW VAULT PASSWORD"; color: "#8E9AAB"; font.pixelSize: 11; font.bold: true }
                TextField {
                    id: newPassInput
                    Layout.fillWidth: true
                    Layout.preferredHeight: 44
                    color: "white"
                    rightPadding: 40
                    property bool showPass: false
                    selectByMouse: true
                    selectionColor: "#4EA8FE"
                    selectedTextColor: "#FFFFFF"
                    echoMode: showPass ? TextInput.Normal : TextInput.Password
                    background: Rectangle { color: "#000000"; radius: 6; border.color: "#1A2535"; border.width: 1 }

                    // --- Icon Mắt Ẩn/Hiện ---
                    Item {
                        z: 99 // Nổi lên trên cùng để có thể click được
                        anchors.right: parent.right; anchors.rightMargin: 12; anchors.verticalCenter: parent.verticalCenter
                        width: 18; height: 18

                        Image {
                            id: newEyeSvg
                            anchors.fill: parent
                            source: newPassInput.showPass ? "qrc:/View/Icons/visibility.svg" : "qrc:/View/Icons/visibility_off.svg"
                            sourceSize: Qt.size(18, 18)
                            visible: false
                        }
                        ColorOverlay {
                            anchors.fill: newEyeSvg; source: newEyeSvg
                            color: newEyeArea.containsMouse ? "#4EA8FE" : "#8E9AAB"
                        }
                        MouseArea {
                            id: newEyeArea
                            anchors.fill: parent; anchors.margins: -5
                            cursorShape: Qt.PointingHandCursor; hoverEnabled: true
                            onClicked: newPassInput.showPass = !newPassInput.showPass
                        }
                    }
                }
            }

            // Trường Confirm Password
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 8
                Text { text: "CONFIRM NEW PASSWORD"; color: "#8E9AAB"; font.pixelSize: 11; font.bold: true }
                TextField {
                    id: confirmPassInput
                    Layout.fillWidth: true
                    Layout.preferredHeight: 44
                    color: "white"
                    rightPadding: 40
                    property bool showPass: false
                    echoMode: showPass ? TextInput.Normal : TextInput.Password
                    selectByMouse: true
                    selectionColor: "#4EA8FE"
                    selectedTextColor: "#FFFFFF"
                    background: Rectangle {
                        color: "#000000"; radius: 6;
                        border.color: (text !== "" && text === newPassInput.text) ? "#34D399" : "#1A2535"
                        border.width: 1
                    }

                    // --- Icon Mắt Ẩn/Hiện ---
                    Item {
                        z: 99
                        anchors.right: parent.right; anchors.rightMargin: 12; anchors.verticalCenter: parent.verticalCenter
                        width: 18; height: 18

                        Image {
                            id: confirmEyeSvg
                            anchors.fill: parent
                            source: confirmPassInput.showPass ? "qrc:/View/Icons/visibility.svg" : "qrc:/View/Icons/visibility_off.svg"
                            sourceSize: Qt.size(18, 18)
                            visible: false
                        }
                        ColorOverlay {
                            anchors.fill: confirmEyeSvg; source: confirmEyeSvg
                            color: confirmEyeArea.containsMouse ? "#4EA8FE" : "#8E9AAB"
                        }
                        MouseArea {
                            id: confirmEyeArea
                            anchors.fill: parent; anchors.margins: -5
                            cursorShape: Qt.PointingHandCursor; hoverEnabled: true
                            onClicked: confirmPassInput.showPass = !confirmPassInput.showPass
                        }
                    }
                }
            }
        }
    }

    // --- Nút bấm cuối cùng ---
    Button {
        id: resetBtn
        Layout.fillWidth: true
        Layout.preferredHeight: 48
        Layout.bottomMargin: 5
        enabled: isKekVerified && newPassInput.text !== "" && newPassInput.text === confirmPassInput.text

        contentItem: Text {
            text: "Change Password"
            color: parent.enabled ? "black" : "#5E6A7A"
            font.bold: true; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
        }
        background: Rectangle {
            color: parent.enabled ? "#34D399" : "#1A2535"
            radius: 6
        }
        onClicked: resetPasswordRequested(kekInput.text, newPassInput.text)
    }

    // Nút Cancel
    Text {
        text: "Cancel"
        color: "#EF4444"
        font.pixelSize: 13
        font.bold: true
        Layout.bottomMargin: 15
        Layout.alignment: Qt.AlignHCenter
        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: forgetPassPopup.close()
        }
    }
}
