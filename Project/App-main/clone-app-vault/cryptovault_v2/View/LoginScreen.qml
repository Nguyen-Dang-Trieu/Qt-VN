import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: root

    readonly property int marginSize: 10

    // Signal
    signal loginSuccess()
    signal goToRegister()

    anchors.fill: parent

    color: "#1a4c6e"

    // ===== CENTER CONTAINER =====
    Item {
        anchors.fill: parent

        RowLayout {
            anchors.centerIn: parent
            width: Math.min(parent.width - 2 * marginSize, 900)
            height: Math.min(parent.height - 2 * marginSize, 600)
            spacing: 0

            // ===== LEFT: FORM =====
            Rectangle {
                Layout.preferredWidth: 380
                Layout.fillHeight: true
                color: "#3b5388"
                radius: 12

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 30
                    spacing: 15

                    // Logo
                    RowLayout {
                        Layout.alignment: Qt.AlignCenter
                        spacing: 10

                        // icon
                        Rectangle {
                            width: 36
                            height: 36
                            radius: 8
                            color: "#2D7CF1"
                        }

                        Label {
                            text: "Cryto Vault"
                            font.pixelSize: 24
                            font.bold: true
                            color: "white"
                            // verticalAlignment: Text.AlignVCenter

                        }
                    }

                    // Empty space
                    Item { Layout.preferredHeight: 10 }

                    /* Account to login */
                    // Username
                    Label { text: "Username"; color: "#ddd" }

                    TextField {
                        id: username
                        // placeholderText: "Username"
                        Layout.fillWidth: true
                        height: 40
                        leftPadding: 40

                        background: Rectangle {
                            radius: 8
                            border.color: username.activeFocus ? "#2D7CF1" : "#ccc"
                            color: "white"
                        }

                        Label {
                            text: "👤"
                            anchors.left: parent.left
                            anchors.leftMargin: 12
                            anchors.verticalCenter: parent.verticalCenter
                            color: "#999"
                        }
                    }

                    // Password
                    Label { text: "Password"; color: "#ddd" }

                    TextField {
                        id: pass
                        echoMode: TextField.Password
                        Layout.fillWidth: true
                        height: 40
                        leftPadding: 40

                        background: Rectangle {
                            radius: 8
                            border.color: pass.activeFocus ? "#2D7CF1" : "#ccc"
                            color: "white"
                        }

                        Label {
                            text: "🔑"
                            anchors.left: parent.left
                            anchors.leftMargin: 12
                            anchors.verticalCenter: parent.verticalCenter
                            color: "#999"
                        }
                    }

                    /* Button: Sign In */
                    Button {
                        text: "Sign In"
                        Layout.fillWidth: true
                        height: 45

                        background: Rectangle {
                            color: "#2D7CF1"
                            radius: 8
                        }

                        contentItem: Text {
                            text: parent.text
                            color: "white"
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        onClicked: root.loginSuccess()
                    }

                    /* --- Divider --- */
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10

                        Rectangle { Layout.fillWidth: true; height: 1; color: "#aaa" }
                        Label { text: "OR"; color: "#ccc" }
                        Rectangle { Layout.fillWidth: true; height: 1; color: "#aaa" }
                    }


                    /* --- Sign Up --- */
                    Text {
                        text: "Don't have an account? <font color='#2D7CF1'>Sign up</font>"
                        color: "#ddd"
                        Layout.alignment: Qt.AlignHCenter

                        MouseArea {
                            anchors.fill: parent
                            onClicked: root.goToRegister()
                        }
                    }

                    Item { Layout.fillHeight: true }
                }
            }

            // ===== RIGHT: IMAGE =====
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "transparent"

                Image {
                    anchors.fill: parent
                    source: "Images/background.png"
                    fillMode: Image.PreserveAspectFit
                }
            }
        }
    }
}
