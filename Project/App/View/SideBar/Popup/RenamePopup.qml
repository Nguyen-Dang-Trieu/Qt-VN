// RenamePopup.qml
// add start 20261004 Criz
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Popup {
    id: root

    property string currentName: ""
    property color primaryBlue: "#246a99"
    property color hoverBlue: "#2d82bc"

    signal renameConfirmed(string newName)

    parent: Overlay.overlay
    x: Math.round((parent.width - width) / 2)
    y: Math.round((parent.height - height) / 2)
    width: 260
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    background: Rectangle {
        color: "#1e293b"
        radius: 10
        border.color: "#334155"
    }

    Component.onCompleted: {
        renameInput.text = currentName
        renameInput.forceActiveFocus()
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 15
        spacing: 15

        Text { text: "Rename Vault"; color: "white"; font.bold: true; font.pixelSize: 15 }

        TextField {
            id: renameInput
            Layout.fillWidth: true
            color: "white"
            background: Rectangle {
                color: "#0f172a"
                radius: 6
                border.color: parent.activeFocus ? root.primaryBlue : "#334155"
            }
            onAccepted: saveBtn.clicked()
        }

        Button {
            id: saveBtn
            Layout.fillWidth: true
            Layout.bottomMargin: 15;
            height: 35
            text: "Save Changes"

            contentItem: Text { text: parent.text; color: "white"; horizontalAlignment: Text.AlignHCenter }
            background: Rectangle {
                color: parent.hovered ? root.hoverBlue : root.primaryBlue
                radius: 6
            }

            onClicked: {
                let newName = renameInput.text.trim()
                if (newName !== "" && newName !== root.currentName) {
                    root.renameConfirmed(newName)
                }
            }
        }
    }
}
// add end 20261004 Criz
