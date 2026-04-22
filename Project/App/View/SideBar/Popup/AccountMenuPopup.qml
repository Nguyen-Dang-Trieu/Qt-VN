// File: AccountMenuPopup.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Popup {
    id: accountMenuRoot
    width: 200
    padding: 4

    closePolicy: Popup.CloseOnPressOutside | Popup.CloseOnEscape

    signal changeAccountPasswordRequested()
    signal logoutRequested()

    background: Rectangle {
        color: "#0D1321"         // Nền tối chuẩn
        border.color: "#1A2535"  // Viền chuẩn
        border.width: 1
        radius: 8
    }

    contentItem: ColumnLayout {
        spacing: 2

        // --------------------------- CHANGE ACCOUNT PASSWORD ---------------------------
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 36
            radius: 6
            color: changePassMouse.containsMouse ? "#121A2F" : "transparent"

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 10
                spacing: 10

                Text {
                    text: "Change Account Password"
                    color: changePassMouse.containsMouse ? "#4EA8FE" : "white"
                    font.pixelSize: 13
                    font.bold: true
                    verticalAlignment: Text.AlignVCenter
                }
            }
            MouseArea {
                id: changePassMouse
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    accountMenuRoot.changeAccountPasswordRequested()
                    accountMenuRoot.close()
                }
            }
        }

        // --------------------------- LINE ---------------------------
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 1
            color: "#1A2535"
            Layout.topMargin: 2
            Layout.bottomMargin: 2
        }

        // --------------------------- LOG OUT ---------------------------
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 36
            radius: 6
            color: logoutMouse.containsMouse ? "#2A1215" : "transparent"

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 10
                spacing: 10

                Text {
                    text: "Log Out"
                    color: logoutMouse.containsMouse ? "#EF4444" : "#475569"
                    font.pixelSize: 13
                    font.bold: true
                    verticalAlignment: Text.AlignVCenter
                }
            }
            MouseArea {
                id: logoutMouse
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    accountMenuRoot.logoutRequested() // Phát tín hiệu Log Out
                    accountMenuRoot.close()
                }
            }
        }
    }
}
