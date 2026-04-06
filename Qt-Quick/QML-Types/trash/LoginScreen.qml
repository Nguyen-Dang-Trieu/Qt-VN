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

    color: "red" // "#f5f7fb"

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

                    Item { Layout.preferredHeight: 10 }

                    // Email
                    Label { text: "Email"; color: "#ddd" }

                    TextField {
                        id: email
                        placeholderText: "airspace@info.com"
                        Layout.fillWidth: true
                        height: 40
                        leftPadding: 40

                        background: Rectangle {
                            radius: 8
                            border.color: email.activeFocus ? "#2D7CF1" : "#ccc"
                            color: "white"
                        }

                        Label {
                            text: "✉"
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

                    CheckBox {
                        text: "Remember me"
                        contentItem: Text {
                            text: parent.text
                            color: "#ddd"
                            verticalAlignment: Text.AlignVCenter
                            leftPadding: 6
                        }
                    }

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

                    // Divider
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10

                        Rectangle { Layout.fillWidth: true; height: 1; color: "#aaa" }
                        Label { text: "OR"; color: "#ccc" }
                        Rectangle { Layout.fillWidth: true; height: 1; color: "#aaa" }
                    }

                    Button {
                        text: "Sign in with Google"
                        Layout.fillWidth: true
                        height: 40

                        background: Rectangle {
                            border.color: "#ccc"
                            radius: 8
                            color: "transparent"
                        }
                    }

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
                    source: "file:///D:/Qt/Work/UI-APP/background.jpg"
                    fillMode: Image.PreserveAspectFit
                }
            }
        }
    }
}
