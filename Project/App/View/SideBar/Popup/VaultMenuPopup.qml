// File: VaultMenuPopup.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Popup {
    id: vaultMenuRoot
    width: 190
    padding: 4

    // Đảm bảo Popup tự động đóng khi click ra ngoài
    closePolicy: Popup.CloseOnPressOutside | Popup.CloseOnEscape

    property int targetIndex: -1

    // Tín hiệu báo cho lớp trên biết người dùng muốn đổi mật khẩu
    signal changePasswordRequested(int index)
    signal forgetPasswordRequested(int index)

    background: Rectangle {
        color: "#0D1321"         // Nền tối chuẩn
        border.color: "#1A2535"  // Viền chuẩn
        border.width: 1
        radius: 8
    }

    contentItem: ColumnLayout {
        spacing: 2

        // --------------------------- CHANGE PASSWORD ---------------------------
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
                    text: "🔑"
                    font.pixelSize: 14
                }
                Text {
                    text: "Change Password"
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
                    vaultMenuRoot.changePasswordRequested(vaultMenuRoot.targetIndex)
                    vaultMenuRoot.close()
                }
            }
        }

        // --------------------------- FORGOT PASSWORD ---------------------------
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 36
            radius: 6
            color: forgetPassMouse.containsMouse ? "#121A2F" : "transparent"

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 10
                spacing: 10
                Text {
                    text: "🔑"
                    font.pixelSize: 14
                }
                Text {
                    text: "Forgot Password"
                    color: forgetPassMouse.containsMouse ? "#4EA8FE" : "white"
                    font.pixelSize: 13
                    font.bold: true
                    verticalAlignment: Text.AlignVCenter
                }
            }
            MouseArea {
                id: forgetPassMouse
                anchors.fill: parent
                hoverEnabled: true; cursorShape: Qt.PointingHandCursor
                onClicked: {
                    vaultMenuRoot.forgetPasswordRequested(vaultMenuRoot.targetIndex)
                    vaultMenuRoot.close()
                }
            }
        }
    }
}
