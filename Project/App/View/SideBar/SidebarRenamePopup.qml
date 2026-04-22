import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Popup {
    id: renamePopup
    property color primaryBlue
    property color hoverBlue
    property var vaultModel
    property int selectedVaultIndex

    function openPopup(currentName) {
        renameInput.text = currentName
        renamePopup.open()
    }

    parent: Overlay.overlay
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    width: 260; modal: true; focus: true
    background: Rectangle { color: "#1e293b"; radius: 10; border.color: "#334155" }

    ColumnLayout {
        anchors.fill: parent; anchors.margins: 15; spacing: 15
        Text { text: "Rename Vault"; color: "white"; font.bold: true; font.pixelSize: 15 }

        TextField {
            id: renameInput
            Layout.fillWidth: true; color: "white"; focus: true
            background: Rectangle { color: "#0f172a"; radius: 6; border.color: parent.activeFocus ? primaryBlue : "#334155" }
        }

        Button {
            Layout.fillWidth: true; height: 35; text: "Save Changes"
            contentItem: Text { text: parent.text; color: "white"; horizontalAlignment: Text.AlignHCenter }
            background: Rectangle { color: parent.hovered ? hoverBlue : primaryBlue; radius: 6 }
            onClicked: {
                if (renameInput.text.trim() !== "") {
                    vaultModel.setProperty(selectedVaultIndex, "name", renameInput.text.trim())
                    renamePopup.close()
                }
            }
        }
    }
}
