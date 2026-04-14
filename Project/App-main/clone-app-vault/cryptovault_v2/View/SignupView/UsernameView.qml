import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: rootCreateUsername
    width: 1280
    height: 720
    color: "#060913"

    signal backToLoginRequested()
    signal nextToCreatePasswordRequested()
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
                        text: "STEP 1 OF 2"
                        color: "#4EA8FE"
                        font.pixelSize: 11
                        font.bold: true
                        font.letterSpacing: 1.5
                    }
                    Rectangle {
                        Layout.fillWidth: true
                        height: 1
                        color: "#1A2535"
                        Rectangle {
                            width: parent.width * 0.5
                            height: 1
                            color: "#4EA8FE"
                        }
                    }
                }

                // --- Titles ---
                ColumnLayout {
                    spacing: 15
                    Text {
                        text: "Create Username"
                        color: "white"
                        font.pixelSize: 42
                        font.bold: true
                        textFormat: Text.RichText
                        lineHeight: 1.1
                    }
                }

                Item { Layout.preferredHeight: 10 }

                // --- Input Field Area ---
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 8

                    RowLayout {
                        Layout.fillWidth: true
                        Text {
                            text: "USERNAME"
                            color: "#8E9AAB"
                            font.pixelSize: 11
                            font.bold: true
                            font.letterSpacing: 1
                        }

                        Item { Layout.fillWidth: true }


                        Text {
                            text: "Maximum 20 characters"
                            color: "#5E6A7A"
                            font.pixelSize: 11
                            font.italic: true
                        }
                    }

                    // TextField Custom
                    TextField {
                        id: usernameInput
                        color: "white"

                        selectByMouse: true
                        selectionColor: "#4EA8FE"
                        selectedTextColor: "#FFFFFF"

                        placeholderText: "e.g: crypto_explorer"

                        validator: RegularExpressionValidator {
                            regularExpression: /^[a-zA-Z0-9_]*$/
                        }

                        Layout.fillWidth: true
                        Layout.preferredHeight: 56
                        maximumLength: 20
                        font.pixelSize: 15
                        leftPadding: 45
                        rightPadding: 45

                        background: Rectangle {
                            color: "#000000"
                            radius: 8
                            border.color: usernameInput.activeFocus ? "#4EA8FE" : "#1A2535"
                            border.width: 1
                        }

                        // Icon '@'
                        Text {
                            text: "@"
                            color: "#5E6A7A"
                            font.pixelSize: 18
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
                            anchors.rightMargin: 15
                            anchors.verticalCenter: parent.verticalCenter
                            visible: usernameInput.text.length > 3

                            Text {
                                text: "✓"
                                color: "#000000"
                                font.pixelSize: 12
                                font.bold: true
                                anchors.centerIn: parent
                            }
                        }
                    }

                    // Status Message
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 6

                        // Icon
                        Rectangle {
                            width: 6
                            height: 6
                            radius: 3
                            color: usernameInput.text.length > 3 ? "#34D399" : "transparent"
                        }

                        // Statuc Text
                        Text {
                            text: usernameInput.text.length > 3 ? "USERNAME AVAILABLE" : ""
                            color: "#34D399"
                            font.pixelSize: 11
                            font.bold: true
                        }

                        Item { Layout.fillWidth: true }

                        Text {
                            text: usernameInput.text.length + "/20"
                            color: usernameInput.text.length >= 20 ? "#EF4444" : "#5E6A7A" // Change color when limited
                            font.pixelSize: 11
                            visible: usernameInput.text.length > 0
                        }
                    }
                }

                Item { Layout.preferredHeight: 30 }

                // --- Footer Actions ---

                //Button Back
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
                                rootCreateUsername.backToLoginRequested()
                            }
                        }
                    }
                    Item { Layout.fillWidth: true }

                    // Button "Next Step"
                    Button {
                        Layout.preferredWidth: 160
                        Layout.preferredHeight: 48
                        enabled: usernameInput.text.length > 3
                        contentItem: RowLayout {
                            anchors.centerIn: parent
                            spacing: 10
                            Text {
                                text: "NEXT STEP"
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
                            console.log("Proceed to Security Step with username:", usernameInput.text)
                            rootCreateUsername.nextToCreatePasswordRequested()
                        }
                    }
                }
            }
        }
    }
}
