import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: registerRoot
    color: Style.bgPrimary
    anchors.fill: parent

    // --- THêm signal
    signal backToLogin()

    ColumnLayout {
        anchors.centerIn: parent
        width: 400
        spacing: 25

        Label { text: "Create Account"; font.pixelSize: 24; color: "white"; Layout.alignment: Qt.AlignHCenter }

        Rectangle {
            Layout.fillWidth: true; Layout.preferredHeight: 300
            color: Style.bgSecondary; radius: 10
            border.color: "#30363D"

            ColumnLayout {
                anchors.fill: parent; anchors.margins: 30
                spacing: 15

                Label { text: "Choose your username"; color: Style.textSecondary }
                TextField {
                    Layout.fillWidth: true
                    placeholderText: "Username..."
                    background: Rectangle { color: "#0D1117"; radius: 5 }
                }

                Button {
                    text: "Continue to Password"
                    Layout.fillWidth: true
                    background: Rectangle { color: Style.accentBlue; radius: 5 }
                    onClicked: console.log("Next step...")
                }

                Text {
                    text: "Back to Login"
                    color: Style.textSecondary
                    Layout.alignment: Qt.AlignHCenter
                    MouseArea {
                        anchors.fill: parent
                        onClicked: registerRoot.backToLogin() // PHÁT SIGNAL
                    }
                }
            }
        }

        // Stepper dưới cùng
        Row {
            spacing: 20; Layout.alignment: Qt.AlignHCenter
            Repeater {
                model: 3
                Rectangle {
                    width: 40; height: 40; radius: 20
                    color: index === 0 ? Style.accentBlue : "#30363D"
                    Text { text: index + 1; anchors.centerIn: parent; color: "white" }
                }
            }
        }
    }
}
