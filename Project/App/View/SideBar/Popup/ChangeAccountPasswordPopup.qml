import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

Popup {
    id: changeAccountPassPopup
    width: 480
    modal: true
    focus: true
    anchors.centerIn: Overlay.overlay

    // --- Signals & Properties ---
    signal changeAccountPasswordRequested(string currentPass, string newPass)
    property bool isError: false
    property bool isSuccess: false

    function showError(msg) {
        statusMsg.text = msg; statusMsg.color = "#ef4444"
        isError = true; isSuccess = false; statusMsg.visible = true
    }

    function showSuccess(msg) {
        statusMsg.text = msg; statusMsg.color = "#34D399"
        isError = false; isSuccess = true; statusMsg.visible = true
        currentPassInput.enabled = false; newPassInput.enabled = false
        confirmPassInput.enabled = false; confirmBtn.enabled = false
    }

    function calculateStrength(pass) {
        if (pass.length === 0) return 0;
        let score = 0;
        if (pass.length >= 16) score++;
        if (/[A-Z]/.test(pass) && /[a-z]/.test(pass)) score++;
        if (/\d/.test(pass) && /[^A-Za-z0-9]/.test(pass)) score++;
        return score;
    }
    property int passStrength: calculateStrength(newPassInput.text)

    background: Rectangle {
        color: "#0f172a"
        radius: 12
        border.color: "#1e293b"
        border.width: 1
    }

    contentItem: ColumnLayout {
        anchors.fill: parent
        anchors.margins: 30
        spacing: 15

        Text { text: "Change Account Password"; color: "white"; font.pixelSize: 22; font.bold: true; Layout.alignment: Qt.AlignHCenter }
        Text { text: "For User: " + authManager.getActiveUsername(); color: "#94a3b8"; font.pixelSize: 13; Layout.alignment: Qt.AlignHCenter; Layout.bottomMargin: 10 }

        // --- CURRENT PASSWORD ---
        ColumnLayout {
            Layout.fillWidth: true; spacing: 8
            Text { text: "CURRENT PASSWORD"; color: "#8E9AAB"; font.pixelSize: 11; font.bold: true }
            TextField {
                id: currentPassInput
                Layout.fillWidth: true; Layout.preferredHeight: 44; color: "white"
                rightPadding: 40
                property bool showPass: false
                echoMode: showPass ? TextInput.Normal : TextInput.Password
                placeholderText: "Enter current password"; placeholderTextColor: "#5E6A7A"
                background: Rectangle { color: "#1e293b"; radius: 8; border.color: parent.activeFocus ? "#0284c7" : "#334155" }
                onTextChanged: if (isError) { statusMsg.visible = false; isError = false }

                // Icon Mắt Ẩn/Hiện
                Item {
                    z: 99
                    anchors.right: parent.right;
                    anchors.rightMargin: 12;
                    anchors.verticalCenter: parent.verticalCenter
                    width: 18;
                    height: 18
                    Image {
                        id: currentEyeSvg
                        anchors.fill: parent
                        source: currentPassInput.showPass ? "qrc:/View/Icons/visibility.svg" : "qrc:/View/Icons/visibility_off.svg"
                        sourceSize: Qt.size(18, 18)
                        visible: false
                    }
                    ColorOverlay {
                        anchors.fill: currentEyeSvg; source: currentEyeSvg
                        color: currentEyeArea.containsMouse ? "#4EA8FE" : "#8E9AAB"
                    }
                    MouseArea {
                        id: currentEyeArea
                        anchors.fill: parent; anchors.margins: -5
                        cursorShape: Qt.PointingHandCursor; hoverEnabled: true
                        onClicked: currentPassInput.showPass = !currentPassInput.showPass
                    }
                }
            }
        }

        // --- NEW PASSWORD  ---
        ColumnLayout {
            Layout.fillWidth: true; spacing: 8
            Text { text: "NEW PASSWORD"; color: "#8E9AAB"; font.pixelSize: 11; font.bold: true }
            TextField {
                id: newPassInput
                Layout.fillWidth: true; Layout.preferredHeight: 44; color: "white"
                rightPadding: 40
                property bool showPass: false
                echoMode: showPass ? TextInput.Normal : TextInput.Password
                placeholderText: "Min 16 characters (Strong required)"; placeholderTextColor: "#5E6A7A"
                background: Rectangle { color: "#1e293b"; radius: 8; border.color: parent.activeFocus ? "#0284c7" : "#334155" }
                onTextChanged: if (isError) { statusMsg.visible = false; isError = false }

                // Icon Mắt Ẩn/Hiện
                Item {
                    z: 99
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

            RowLayout {
                Layout.fillWidth: true; spacing: 5; visible: newPassInput.text.length > 0
                Text { text: passStrength <= 1 ? "WEAK" : (passStrength === 2 ? "MEDIUM" : "STRONG"); color: passStrength <= 1 ? "#EF4444" : (passStrength === 2 ? "#F59E0B" : "#34D399"); font.pixelSize: 10; font.bold: true; Layout.preferredWidth: 50 }
                RowLayout {
                    Layout.fillWidth: true; spacing: 4
                    Rectangle { Layout.fillWidth: true; height: 4; radius: 2; color: passStrength >= 1 ? (passStrength === 1 ? "#EF4444" : (passStrength === 2 ? "#F59E0B" : "#34D399")) : "#1A2535" }
                    Rectangle { Layout.fillWidth: true; height: 4; radius: 2; color: passStrength >= 2 ? (passStrength === 2 ? "#F59E0B" : "#34D399") : "#1A2535" }
                    Rectangle { Layout.fillWidth: true; height: 4; radius: 2; color: passStrength >= 3 ? "#34D399" : "#1A2535" }
                }
            }
        }

        // --- CONFIRM PASSWORD ---
        ColumnLayout {
            Layout.fillWidth: true; spacing: 8
            Text { text: "CONFIRM NEW PASSWORD"; color: "#8E9AAB"; font.pixelSize: 11; font.bold: true }
            TextField {
                id: confirmPassInput
                Layout.fillWidth: true; Layout.preferredHeight: 44; color: "white"
                rightPadding: 40
                property bool showPass: false
                echoMode: showPass ? TextInput.Normal : TextInput.Password
                placeholderText: "Re-enter new password"; placeholderTextColor: "#5E6A7A"
                property bool isMatch: text.length > 0 && text === newPassInput.text
                background: Rectangle { color: "#1e293b"; radius: 8; border.color: confirmPassInput.isMatch ? "#34D399" : (confirmPassInput.text.length > 0 && !confirmPassInput.isMatch ? "#ef4444" : (parent.activeFocus ? "#0284c7" : "#334155")) }
                onTextChanged: if (isError) { statusMsg.visible = false; isError = false }
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
            Text { text: "Passwords do not match!"; color: "#ef4444"; font.pixelSize: 11; visible: confirmPassInput.text.length > 0 && !confirmPassInput.isMatch }
        }

        Text { id: statusMsg; text: ""; font.pixelSize: 12; visible: false; Layout.alignment: Qt.AlignHCenter }

        // --- ACTION BUTTONS ---
        RowLayout {
            Layout.fillWidth: true
            Layout.topMargin: 10
            spacing: 15
            Layout.bottomMargin: 20

            Button {
                Layout.fillWidth: true; Layout.preferredHeight: 44
                contentItem: Text { text: "Cancel"; color: "#94a3b8"; font.bold: true; font.pixelSize: 14; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
                background: Rectangle { color: parent.hovered ? "#334155" : "transparent"; border.color: "#334155"; border.width: 1; radius: 8 }
                onClicked: changeAccountPassPopup.close()
            }
            Button {
                id: confirmBtn
                Layout.fillWidth: true; Layout.preferredHeight: 44
                enabled: currentPassInput.text !== "" && newPassInput.text.length >= 16 && passStrength === 3 && confirmPassInput.isMatch
                contentItem: Text { text: "Change Password"; color: parent.enabled ? "white" : "#94a3b8"; font.bold: true; font.pixelSize: 14; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
                background: Rectangle { color: isSuccess ? "#059669" : (!parent.enabled ? "#1e293b" : (parent.pressed ? "#0369a1" : "#0284c7")); radius: 8 }
                onClicked: changeAccountPassPopup.changeAccountPasswordRequested(currentPassInput.text, newPassInput.text)
            }
        }
    }
    onOpened: currentPassInput.forceActiveFocus()
}
