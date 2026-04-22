import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

Popup {
    id: forgetAccountPopup
    width: 480
    modal: true
    focus: true
    anchors.centerIn: Overlay.overlay

    // --- Properties ---
    property bool isKekVerified: false
    property string targetUsername: ""
    property bool isKekInvalid: false

    function showKekError() {
        isKekInvalid = true;
    }

    // CẬP NHẬT: Hàm tính toán độ khó với tiêu chuẩn 16 ký tự
    function calculateStrength(pass) {
        if (pass.length === 0) return 0;
        let score = 0;

        if (pass.length >= 16) score++;
        if (/[A-Z]/.test(pass) && /[a-z]/.test(pass)) score++;
        if (/\d/.test(pass) && /[^A-Za-z0-9]/.test(pass)) score++;
        return score;
    }

    property int passStrength: calculateStrength(newPassInput.text)

    // --- Signals ---
    signal verifyAccountKekRequested(string username, string kek)
    signal resetAccountPasswordRequested(string username, string kek, string newPass)

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
                text: "Account Recovery"
                color: "white"
                font.pixelSize: 22
                font.bold: true
                Layout.alignment: Qt.AlignHCenter
            }
            Text {
                text: "Enter your Username and Account Recovery Key to reset your login password."
                color: "#8E9AAB"
                font.pixelSize: 13
                wrapMode: Text.Wrap
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
                lineHeight: 1.4
            }
        }

        // --- GIAI ĐOẠN 1: NHẬP USERNAME & KEK ---
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 12

            // Ô nhập Username
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 8
                Text { text: "USERNAME"; color: "#8E9AAB"; font.pixelSize: 11; font.bold: true; font.letterSpacing: 1 }

                TextField {
                    id: usernameInput
                    text: targetUsername
                    Layout.fillWidth: true
                    Layout.preferredHeight: 44
                    color: isKekVerified ? "#34D399" : "white"
                    placeholderText: "Enter your account username..."
                    placeholderTextColor: "#5E6A7A"
                    readOnly: isKekVerified

                    // THÊM: Ẩn lỗi khi người dùng bắt đầu gõ lại
                    onTextChanged: isKekInvalid = false

                    background: Rectangle {
                        color: "#000000"
                        radius: 6
                        // CẬP NHẬT: Hiển thị viền đỏ nếu isKekInvalid = true
                        border.color: isKekVerified ? "#34D399" : (isKekInvalid ? "#EF4444" : (parent.activeFocus ? "#4EA8FE" : "#1A2535"))
                        border.width: 1
                    }
                }
            }

            // Ô nhập KEK
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 8
                Text { text: "ACCOUNT RECOVERY KEY"; color: "#8E9AAB"; font.pixelSize: 11; font.bold: true; font.letterSpacing: 1 }

                TextField {
                    id: kekInput
                    Layout.fillWidth: true
                    Layout.preferredHeight: 44
                    color: isKekVerified ? "#34D399" : "white"
                    placeholderText: "Enter your 64-character key..."
                    placeholderTextColor: "#5E6A7A"
                    readOnly: isKekVerified

                    // THÊM: Ẩn lỗi khi người dùng bắt đầu gõ lại
                    onTextChanged: isKekInvalid = false

                    background: Rectangle {
                        color: "#000000"
                        radius: 6
                        // CẬP NHẬT: Hiển thị viền đỏ nếu isKekInvalid = true
                        border.color: isKekVerified ? "#34D399" : (isKekInvalid ? "#EF4444" : (parent.activeFocus ? "#4EA8FE" : "#1A2535"))
                        border.width: 1
                    }
                }
            }

            // THÊM: Dòng text báo lỗi đỏ chót
            Text {
                text: "Invalid Username or Recovery Key!"
                color: "#EF4444"
                font.pixelSize: 12
                font.bold: true
                visible: isKekInvalid // Chỉ hiện khi có lỗi
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 2
            }

            Button {
                id: verifyBtn
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                Layout.topMargin: 5
                visible: !isKekVerified
                enabled: kekInput.text.length == 64 && usernameInput.text.length > 0

                contentItem: Text {
                    text: "Verify Key"
                    color: parent.enabled ? "black" : "#5E6A7A"
                    font.bold: true; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
                }
                background: Rectangle {
                    color: parent.enabled ? (parent.pressed ? "#3A80C9" : "#4EA8FE") : "#1A2535"
                    radius: 6
                }
                onClicked: verifyAccountKekRequested(usernameInput.text, kekInput.text)
            }
        }

        // --- GIAI ĐOẠN 2: NHẬP MẬT KHẨU ĐĂNG NHẬP MỚI ---
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 15
            opacity: isKekVerified ? 1.0 : 0.3
            enabled: isKekVerified

            // Trường New Password
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 8
                Text { text: "NEW LOGIN PASSWORD"; color: "#8E9AAB"; font.pixelSize: 11; font.bold: true }

                TextField {
                    id: newPassInput
                    Layout.fillWidth: true
                    Layout.preferredHeight: 44
                    color: "white"
                    placeholderText: "Min 16 characters (Strong required)"
                    placeholderTextColor: "#5E6A7A"

                    property bool showPass: false
                    echoMode: showPass ? TextInput.Normal : TextInput.Password

                    background: Rectangle { color: "#000000"; radius: 6; border.color: parent.activeFocus ? "#4EA8FE" : "#1A2535"; border.width: 1 }

                    Item {
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

                // THANH HIỂN THỊ ĐỘ KHÓ
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 5
                    visible: newPassInput.text.length > 0

                    Text {
                        text: {
                            if (passStrength <= 1) return "WEAK"
                            if (passStrength === 2) return "MEDIUM"
                            return "STRONG"
                        }
                        color: {
                            if (passStrength <= 1) return "#EF4444"
                            if (passStrength === 2) return "#F59E0B"
                            return "#34D399"
                        }
                        font.pixelSize: 10
                        font.bold: true
                        Layout.preferredWidth: 50
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 4
                        Rectangle { Layout.fillWidth: true; height: 4; radius: 2; color: passStrength >= 1 ? (passStrength === 1 ? "#EF4444" : (passStrength === 2 ? "#F59E0B" : "#34D399")) : "#1A2535" }
                        Rectangle { Layout.fillWidth: true; height: 4; radius: 2; color: passStrength >= 2 ? (passStrength === 2 ? "#F59E0B" : "#34D399") : "#1A2535" }
                        Rectangle { Layout.fillWidth: true; height: 4; radius: 2; color: passStrength >= 3 ? "#34D399" : "#1A2535" }
                    }
                }

                // Mẹo nhỏ báo lỗi độ dài ngay dưới thanh strength
                Text {
                    text: "Need at least " + (16 - newPassInput.text.length) + " more characters"
                    color: "#EF4444"
                    font.pixelSize: 10
                    visible: newPassInput.text.length > 0 && newPassInput.text.length < 16
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
                    placeholderText: "Re-enter new password"
                    placeholderTextColor: "#5E6A7A"

                    property bool showPass: false
                    echoMode: showPass ? TextInput.Normal : TextInput.Password

                    background: Rectangle {
                        color: "#000000"; radius: 6;
                        border.color: (confirmPassInput.text !== "" && confirmPassInput.text === newPassInput.text) ? "#34D399" : (parent.activeFocus ? "#4EA8FE" : "#1A2535")
                        border.width: 1
                    }

                    Item {
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

                Text {
                    text: "Passwords do not match!"
                    color: "#EF4444"
                    font.pixelSize: 11
                    visible: confirmPassInput.text.length > 0 && confirmPassInput.text !== newPassInput.text
                }
            }
        }

        // --- NÚT BẤM CUỐI CÙNG ---
        Button {
            id: resetBtn
            Layout.fillWidth: true
            Layout.preferredHeight: 48
            Layout.bottomMargin: 5

            // CẬP NHẬT ĐIỀU KIỆN: KEK OK + Độ dài >= 16 + Strength là Mạnh (3) + Khớp pass
            enabled: isKekVerified &&
                     newPassInput.text.length >= 16 &&
                     passStrength === 3 &&
                     newPassInput.text === confirmPassInput.text

            contentItem: Text {
                text: "Change Password"
                color: parent.enabled ? "black" : "#5E6A7A"
                font.bold: true; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle {
                color: parent.enabled ? "#34D399" : "#1A2535"
                radius: 6
            }
            onClicked: resetAccountPasswordRequested(usernameInput.text, kekInput.text, newPassInput.text)
        }

        // --- NÚT CANCEL ---
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
                onClicked: forgetAccountPopup.close()
            }
        }
    }
}
