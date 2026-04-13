import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: root
    width: 1280
    height: 720
    color: "#080B12"

    // Signal
    signal loginSuccess()
    signal goToRegister()

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
                    text: "CryptVault"
                    color: "white"
                    font.pixelSize: 22
                    font.bold: true
                    Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                }

                Item { Layout.fillHeight: true }

                // Main Title
                Text {
                    text: "Enter the <font color='#4EA8FE'>Vault</font>"
                    color: "white"
                    font.pixelSize: 54
                    font.bold: true
                    textFormat: Text.RichText
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
                        source: "Images/background.png"
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

                Text {
                    text: "Welcome Back"
                    color: "white"
                    font.pixelSize: 32
                    font.bold: true
                }

                Text {
                    text: "Logn in to manage your account."
                    color: "#8E9AAB"
                    font.pixelSize: 15
                    lineHeight: 1.4
                    Layout.bottomMargin: 15
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

                        background: Rectangle {
                            color: "white"
                            radius: 6
                        }

                        Text { text: "🔒"; anchors.left: parent.left; anchors.leftMargin: 15; anchors.verticalCenter: parent.verticalCenter; color: "#8E9AAB" }
                        Text {
                            id: togglePasswordIcon
                            text: passwordField.echoMode === TextInput.Password ? "👁️" : "🙈"
                            anchors.right: parent.right
                            anchors.rightMargin: 15
                            anchors.verticalCenter: parent.verticalCenter
                            color: "#8E9AAB"

                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    if (passwordField.echoMode === TextInput.Password) {
                                        passwordField.echoMode = TextInput.Normal
                                    } else {
                                        passwordField.echoMode = TextInput.Password
                                    }
                                }
                            }
                        }                    }
                }

                // --- Login to Vault ---
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
                        console.log("Attempting login with:", usernameField.text)
                    }
                }

                Item { Layout.preferredHeight: 15 }

                Text {
                    text: "Don't have an account? <font color='#4EA8FE'><b>Sign up</b></font>"
                    color: "#8E9AAB"
                    font.pixelSize: 13
                    Layout.alignment: Qt.AlignHCenter
                    textFormat: Text.RichText

                    MouseArea {
                        anchors.fill: parent
                        onClicked: root.goToRegister()
                    }
                }
            }

        }
    }
}
