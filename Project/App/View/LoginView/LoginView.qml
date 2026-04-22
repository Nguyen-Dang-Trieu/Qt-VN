import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

Rectangle {
    id: loginRoot

    signal signUpRequested()
    signal loginSuccess()
    signal forgotPasswordRequested()

    // ==========================================================
    //  FORGOT PASSWORD
    // ==========================================================
    onForgotPasswordRequested: {
        let comp = Qt.createComponent("qrc:/View/LoginView/ForgotAccountPasswordPopup.qml")

        if (comp.status === Component.Ready) {
            let popup = comp.createObject(loginRoot, {
                                              "targetUsername": usernameField.text // Tự động điền Username đang gõ dở
                                          })

            // 1. Lắng nghe yêu cầu Verify KEK từ Popup
            popup.verifyAccountKekRequested.connect(function(username, kek) {
                console.log("Yêu cầu xác thực KEK cho tài khoản:", username)
                let isValid = authManager.verifyAccountKEK(username, kek)
                if (isValid) {
                    popup.isKekVerified = true
                } else {
                    console.error("KEK không hợp lệ!")
                    popup.showKekError()
                }
            })

            // 2. Lắng nghe yêu cầu Reset Password từ Popup
            popup.resetAccountPasswordRequested.connect(function(username, kek, newPass) {
                console.log("Yêu cầu Reset Password cho tài khoản:", username)

                let result = authManager.resetAccountPasswordWithKEK(username, kek, newPass)

                if (result.success) {
                    popup.close()
                    let kekComp = Qt.createComponent("qrc:/View/SignupView/CopyAccountKEKPopup.qml")
                    if (kekComp.status === Component.Ready) {
                        let kekPopup = kekComp.createObject(loginRoot, {
                                                                "kekString": result.newKek
                                                            })

                        kekPopup.copyConfirmed.connect(function() {
                            console.log("Đã lưu KEK Account mới an toàn!")
                            kekPopup.close()

                            usernameField.text = username
                            passwordField.text = newPass
                        })

                        kekPopup.open()
                    }
                } else {
                    console.error("Reset mật khẩu thất bại (Lỗi Database)!")
                }
            })
            popup.closed.connect(function() { popup.destroy() })
            popup.open()
        } else {
            console.error("Lỗi tải ForgotAccountPasswordPopup:", comp.errorString())
        }
    }
    // ==========================================================

    color: "#080B12"

    Item {
        id: scaleWrapper
        width: 1280
        height: 720
        anchors.centerIn: parent
        scale: Math.min(loginRoot.width / width, loginRoot.height / height)

        RowLayout {
            anchors.fill: parent
            spacing: 0

            // ==========================================
            // LEFT AREA
            // ==========================================
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredWidth: 60
                color: "#0D1321"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 60
                    spacing: 20

                    // Logo
                    Text {
                        text: "CryptVault V2.0"
                        color: "white"
                        font.pixelSize: 22
                        font.bold: true
                        Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                    }

                    Item { Layout.fillHeight: true }

                    // Main Title
                    Text {
                        text: "Welcome to <font color='#4EA8FE'>Crypt Vault</font>"
                        color: "white"
                        font.pixelSize: 54
                        font.bold: true
                        textFormat: Text.RichText
                        Layout.alignment: Qt.AlignHCenter
                    }

                    // Vault Image
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 500
                        radius: 12
                        color: "#1A2535"
                        clip: true

                        Image {
                            anchors.fill: parent
                            source: "qrc:/View/Images/login.png" // File Path
                            fillMode: Image.PreserveAspectCrop
                        }
                    }

                    Item { Layout.fillHeight: true }
                }
            }

            // ==========================================
            // RIGHT AREA
            // ==========================================
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredWidth: 40
                color: "#080B12"

                ColumnLayout {
                    anchors.centerIn: parent
                    width: 360
                    spacing: 20
                    Image {
                        Layout.alignment: Qt.AlignHCenter
                        Layout.preferredWidth: 100  // Bạn có thể chỉnh to/nhỏ tùy ý
                        Layout.preferredHeight: 100
                        source: "qrc:/View/Images/logo.png"
                        fillMode: Image.PreserveAspectFit
                        mipmap: true // Chống răng cưa cho logo
                    }
//                    Text {
//                        text: ""
//                        color: "white"
//                        font.pixelSize: 32
//                        font.bold: true
//                        Layout.alignment: Qt.AlignHCenter
//                    }
                    //                    Text {
                    //                        text: ""
                    //                        color: "white"
                    //                        font.pixelSize: 32
                    //                        font.bold: true
                    //                        Layout.alignment: Qt.AlignHCenter
                    //                    }

                    Text {
                        text: "Log in to manage your account."
                        color: "#8E9AAB"
                        font.pixelSize: 15
                        lineHeight: 1.4
                        Layout.bottomMargin: 15
                        Layout.alignment: Qt.AlignHCenter
                    }

                    // --- Input: Username ---
                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 6
                        Text { text: "USERNAME"; color: "#8E9AAB"; font.pixelSize: 11; font.bold: true; font.letterSpacing: 1 }

                        TextField {
                            id: usernameField
                            Layout.fillWidth: true
                            Layout.preferredHeight: 48
                            placeholderText: "Enter your username"
                            color: "#000000"
                            font.pixelSize: 14
                            leftPadding: 45

                            selectByMouse: true
                            selectionColor: "#4EA8FE"
                            selectedTextColor: "#FFFFFF"

                            background: Rectangle {
                                color: "white"
                                radius: 6
                            }

                            Text {
                                text: "👤"
                                anchors.left: parent.left
                                anchors.leftMargin: 15
                                anchors.verticalCenter: parent.verticalCenter
                                color: "#8E9AAB"
                            }
                        }
                    }

                    // --- Input: Password ---
                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 6

                        RowLayout {
                            Layout.fillWidth: true
                            Text { text: "PASSWORD"; color: "#8E9AAB"; font.pixelSize: 11; font.bold: true; font.letterSpacing: 1; Layout.fillWidth: true }
                        }

                        TextField {
                            id: passwordField
                            Layout.fillWidth: true
                            Layout.preferredHeight: 48
                            placeholderText: "Enter your password"
                            echoMode: TextInput.Password
                            color: "#000000"
                            font.pixelSize: 14
                            leftPadding: 45
                            rightPadding: 45

                            selectByMouse: true
                            selectionColor: "#4EA8FE"
                            selectedTextColor: "#FFFFFF"

                            background: Rectangle {
                                color: "white"
                                radius: 6
                            }

                            Text {
                                text: "🔒";
                                anchors.left: parent.left;
                                anchors.leftMargin: 15;
                                anchors.verticalCenter: parent.verticalCenter;
                                color: "#8E9AAB"
                            }

                            // --- Icon Visibility ---
                            Item {
                                width: 18
                                height: 18
                                anchors.right: parent.right
                                anchors.rightMargin: 15
                                anchors.verticalCenter: parent.verticalCenter

                                Image {
                                    id: loginEyeSvg
                                    source: passwordField.echoMode === TextInput.Normal ? "qrc:/View/Icons/visibility.svg" : "qrc:/View/Icons/visibility_off.svg"
                                    sourceSize: Qt.size(18, 18)
                                    visible: false
                                }

                                ColorOverlay {
                                    anchors.fill: loginEyeSvg
                                    source: loginEyeSvg
                                    color: eyeMouseArea.containsMouse ? "#4EA8FE" : "#8E9AAB"
                                }

                                MouseArea {
                                    id: eyeMouseArea
                                    anchors.fill: parent
                                    anchors.margins: -10
                                    cursorShape: Qt.PointingHandCursor
                                    hoverEnabled: true
                                    onClicked: {
                                        if (passwordField.echoMode === TextInput.Password) {
                                            passwordField.echoMode = TextInput.Normal
                                        } else {
                                            passwordField.echoMode = TextInput.Password
                                        }
                                    }
                                }
                            }
                        }
                    }

                    // --- Login button ---
                    Text {
                        id: errorMessage
                        text: "Incorrect username or password!"
                        color: "#ef4444"
                        font.pixelSize: 12
                        visible: false
                        Layout.alignment: Qt.AlignHCenter
                    }
                    Button {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 48
                        Layout.topMargin: 10

                        contentItem: Text {
                            text: "Login to Vault →"
                            color: "#000000"
                            font.bold: true
                            font.pixelSize: 14
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        background: Rectangle {
                            color: parent.down ? "#3A80C9" : "#4EA8FE"
                            radius: 6
                        }

                        onClicked: {
                            let success = authManager.handleLogin(usernameField.text, passwordField.text)

                            if (success) {
                                loginRoot.loginSuccess()
                            } else {
                                errorMessage.visible = true // Hiện lỗi
                            }
                        }
                    }

                    Item { Layout.preferredHeight: 15 }

                    Row {
                        Layout.alignment: Qt.AlignHCenter
                        spacing: 5

                        Text {
                            text: "New to Vault?"
                            color: "#8E9AAB"
                            font.pixelSize: 13
                        }

                        Text {
                            text: "Sign up"
                            color: "#4EA8FE"
                            font.pixelSize: 13
                            font.bold: true

                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: loginRoot.signUpRequested()
                            }
                        }
                    }

                    // --- FORGOT PASSWORD ---
                    Text {
                        text: "Forgot Password?"
                        color: forgotMouseArea.containsMouse ? "#4EA8FE" : "#5E6A7A"
                        font.pixelSize: 12
                        font.underline: forgotMouseArea.containsMouse
                        Layout.alignment: Qt.AlignHCenter
                        Layout.topMargin: -10

                        MouseArea {
                            id: forgotMouseArea
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: loginRoot.forgotPasswordRequested()
                        }
                    }
                    // --------------------------------------
                }
            }
        }
    }
}
