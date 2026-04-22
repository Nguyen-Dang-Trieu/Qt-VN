import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

Rectangle {
    id: rootPasswordView
    color: "#060913"

    signal backToUsernameRequested()
    signal signUpCompleted()

    Item {
        id: scaleWrapper
        width: 1280
        height: 720
        anchors.centerIn: parent
        scale: Math.min(rootPasswordView.width / width, rootPasswordView.height / height)

        RowLayout {
            anchors.fill: parent
            spacing: 0

            // ==========================================
            // MAIN CONTENT
            // ==========================================
            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true

                ColumnLayout {
                    anchors.centerIn: parent
                    width: 550
                    spacing: 30

                    // --- Progress Header ---
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 15
                        Text {
                            text: "STEP 2 OF 2"
                            color: "#4EA8FE"
                            font.pixelSize: 11
                            font.bold: true
                            font.letterSpacing: 1.5
                        }
                        Rectangle {
                            height: 1
                            color: "#1A2535"
                            Layout.fillWidth: true
                            Rectangle {
                                width: parent.width
                                height: 1
                                color: "#4EA8FE"
                            }
                        }
                    }

                    // --- Input ---
                    ColumnLayout {
                        spacing: 15
                        Text {
                            text: "Create Password"
                            color: "white"
                            font.pixelSize: 42
                            font.bold: true
                            lineHeight: 1.1
                        }

                        Item { Layout.preferredHeight: 10 }

                        // --- Input Password ---
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 15

                            RowLayout {
                                Layout.fillWidth: true
                                Text {
                                    text: "PASSWORD"
                                    color: "#8E9AAB"
                                    font.pixelSize: 11
                                    font.bold: true
                                    font.letterSpacing: 1
                                }
                                Item { Layout.fillWidth: true }

                                Text {
                                    text: "Min 16 chars includes letters, numbers & special chars"
                                    color: passwordInput.isStrong ? "#34D399" : "#5E6A7A"
                                    font.pixelSize: 11
                                    font.italic: true
                                }
                            }

                            TextField {
                                id: passwordInput
                                property bool isVisible: false

                                property bool isStrong: {
                                    let t = text;
                                    return t.length >= 16 &&
                                            /[a-zA-Z]/.test(t) &&
                                            /[0-9]/.test(t) &&
                                            /[ $@]/.test(t);
                                }

                                color: "white"
                                selectByMouse: true
                                selectionColor: "#4EA8FE"
                                selectedTextColor: "#FFFFFF"
                                placeholderText: "Enter strong password"
                                placeholderTextColor: "#5E6A7A"

                                echoMode: isVisible ? TextInput.Normal : TextInput.Password

                                Layout.fillWidth: true
                                Layout.preferredHeight: 56
                                font.pixelSize: 15
                                leftPadding: 45
                                rightPadding: 45

                                // Icon Lock
                                Text {
                                    text: "🔒"
                                    color: "#5E6A7A"
                                    font.pixelSize: 16
                                    anchors.left: parent.left
                                    anchors.leftMargin: 15
                                    anchors.verticalCenter: parent.verticalCenter
                                }

                                background: Rectangle {
                                    color: "#000000"
                                    radius: 8
                                    border.color: {
                                        if (passwordInput.text.length === 0) {
                                            return passwordInput.activeFocus ? "#4EA8FE" : "#1A2535";
                                        }
                                        return passwordInput.isStrong ? "#34D399" : "#EF4444";
                                    }
                                    border.width: 1
                                }

                                // Visibility, Visibility off Icons
                                Item {
                                    width: 18
                                    height: 18
                                    anchors.right: parent.right
                                    anchors.rightMargin: 15
                                    anchors.verticalCenter: parent.verticalCenter

                                    Image {
                                        id: eyeSvg
                                        source: passwordInput.isVisible ? "qrc:/View/Icons/visibility.svg" : "qrc:/View/Icons/visibility_off.svg"
                                        sourceSize: Qt.size(18, 18)
                                        visible: false
                                    }

                                    ColorOverlay {
                                        anchors.fill: eyeSvg
                                        source: eyeSvg
                                        color: eyeMouseArea.containsMouse ? "#4EA8FE" : "#8E9AAB"
                                    }

                                    MouseArea {
                                        id: eyeMouseArea
                                        anchors.fill: parent
                                        anchors.margins: -10
                                        cursorShape: Qt.PointingHandCursor
                                        hoverEnabled: true
                                        onClicked: passwordInput.isVisible = !passwordInput.isVisible
                                    }
                                }
                                onTextChanged: {
                                    if (errorBox.visible) {
                                        errorBox.visible = false;
                                    }
                                }
                            }

                        }

                        // Input confirm password
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 8

                            Text {
                                text: "CONFIRM PASSWORD"
                                color: "#8E9AAB"
                                font.pixelSize: 11
                                font.bold: true
                                font.letterSpacing: 1
                            }

                            TextField {
                                id: confirmPasswordInput
                                property bool isMatch: text.length > 0 && text === passwordInput.text
                                property bool isVisible: false

                                color: "white"
                                selectByMouse: true
                                selectionColor: "#4EA8FE"
                                selectedTextColor: "#FFFFFF"
                                placeholderText: "Re-enter your password"
                                placeholderTextColor: "#5E6A7A"

                                echoMode: isVisible ? TextInput.Normal : TextInput.Password

                                Layout.fillWidth: true
                                Layout.preferredHeight: 56
                                font.pixelSize: 15
                                leftPadding: 45
                                rightPadding: 45

                                background: Rectangle {
                                    color: "#000000"
                                    radius: 8
                                    border.color: {
                                        if (confirmPasswordInput.isMatch) return "#34D399";
                                        if (confirmPasswordInput.text.length > 0 && confirmPasswordInput.text !== passwordInput.text) return "#EF4444";
                                        if (confirmPasswordInput.activeFocus) return "#4EA8FE";
                                        return "#1A2535";
                                    }
                                    border.width: 1
                                }

                                // Icon Lock
                                Text {
                                    text: "🔒"
                                    color: "#5E6A7A"
                                    font.pixelSize: 16
                                    anchors.left: parent.left
                                    anchors.leftMargin: 15
                                    anchors.verticalCenter: parent.verticalCenter
                                }

                                // Icon Check
                                Rectangle {
                                    width: 20
                                    height: 20
                                    radius: 10
                                    color: "#34D399"
                                    anchors.right: parent.right
                                    anchors.rightMargin: 40
                                    anchors.verticalCenter: parent.verticalCenter
                                    visible: confirmPasswordInput.isMatch

                                    Text {
                                        text: "✓"
                                        color: "#000000"
                                        font.pixelSize: 12
                                        font.bold: true
                                        anchors.centerIn: parent
                                    }
                                }

                                // Visibility, Visibility off Icons
                                Item {
                                    width: 18
                                    height: 18
                                    anchors.right: parent.right
                                    anchors.rightMargin: 15
                                    anchors.verticalCenter: parent.verticalCenter

                                    Image {
                                        id: eyeConfirmPassword
                                        source: confirmPasswordInput.isVisible ? "qrc:/View/Icons/visibility.svg" : "qrc:/View/Icons/visibility_off.svg"
                                        sourceSize: Qt.size(18, 18)
                                        visible: false
                                    }

                                    ColorOverlay {
                                        anchors.fill: eyeConfirmPassword
                                        source: eyeConfirmPassword
                                        color: eyeMouseAreaConfirmPassword.containsMouse ? "#4EA8FE" : "#8E9AAB"
                                    }

                                    MouseArea {
                                        id: eyeMouseAreaConfirmPassword
                                        anchors.fill: parent
                                        anchors.margins: -10
                                        cursorShape: Qt.PointingHandCursor
                                        hoverEnabled: true
                                        onClicked: confirmPasswordInput.isVisible = !confirmPasswordInput.isVisible
                                    }
                                }

                            }

                            Text {
                                text: "Passwords do not match"
                                color: "#EF4444"
                                font.pixelSize: 11
                                visible: confirmPasswordInput.text.length > 0 && !confirmPasswordInput.isMatch
                            }
                        }
                    }

                    Item { Layout.preferredHeight: 30 }
                    // ------------------------------------------------

                    // --- Footer Actions ---
                    RowLayout {
                        Layout.fillWidth: true

                        RowLayout {
                            spacing: 6

                            Text {
                                text: "‹"
                                color: "#8E9AAB"
                                font.pixelSize: 20
                                font.bold: true
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.verticalCenterOffset: -2
                            }

                            Text {
                                text: "BACK"
                                color: "#8E9AAB"
                                font.pixelSize: 12
                                font.bold: true
                                font.letterSpacing: 1
                                anchors.verticalCenter: parent.verticalCenter
                            }

                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    console.log("Back button clicked")
                                    rootPasswordView.backToUsernameRequested()
                                }
                            }
                        }

                        Item { Layout.fillWidth: true }


                        Button {
                            Layout.preferredWidth: 160
                            Layout.preferredHeight: 48
                            enabled: passwordInput.isStrong && confirmPasswordInput.isMatch

                            contentItem: RowLayout {
                                spacing: 8
                                anchors.centerIn: parent
                                Text {
                                    text: "CONFIRM"
                                    color: parent.parent.enabled ? "#000000" : "#5E6A7A"
                                    font.bold: true
                                    font.pixelSize: 13
                                    font.letterSpacing: 1
                                }
                                Text {
                                    text: "›"
                                    color: parent.parent.enabled ? "#000000" : "#5E6A7A"
                                    font.bold: true
                                    font.pixelSize: 18
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.verticalCenterOffset: -1
                                }
                            }

                            background: Rectangle {
                                color: {
                                    if (!parent.enabled) return "#1A2535"
                                    return parent.down ? "#3A80C9" : "#4EA8FE"
                                }
                                radius: 6
                            }

                            onClicked: {
                                // 1. Gọi hàm tạo tài khoản và nhận kết quả
                                let success = authManager.handleSignUp(tempSignupUsername, passwordInput.text)

                                if (success) {
                                    // 2. Lấy KEK của Account từ Model (đã code ở bước trước)
                                    let accountKek = listNodes.getAccountKEK(tempSignupUsername, passwordInput.text)

                                    // 3. Load và hiển thị CopyAccountKEKPopup
                                    // LƯU Ý: Sửa lại đường dẫn này cho đúng với thư mục bạn lưu file CopyAccountKEKPopup.qml
                                    let comp = Qt.createComponent("qrc:/View/SignupView/CopyAccountKEKPopup.qml")

                                    if (comp.status === Component.Ready) {
                                        let popup = comp.createObject(rootPasswordView, {
                                                                          "kekString": accountKek
                                                                      })

                                        // 4. Lắng nghe xác nhận từ Popup
                                        popup.copyConfirmed.connect(function() {
                                            console.log("Đã lưu KEK của Account an toàn!")
                                            rootPasswordView.signUpCompleted()
                                            popup.close()
                                        })

                                        popup.open()
                                    } else {
                                        console.error("Lỗi tải CopyAccountKEKPopup:", comp.errorString())
                                        //rootPasswordView.signUpCompleted()
                                    }
                                } else {
                                    console.log("Đăng ký thất bại. Có thể tài khoản đã tồn tại.")
                                }

                            }
                        }
                    }
                }
            }
        }
    }
}
