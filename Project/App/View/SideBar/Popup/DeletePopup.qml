// DeletePopup.qml
// add start 20261004 Criz
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Popup {
    id: root

    property string vaultName: ""

    signal deleteConfirmed()

    parent: Overlay.overlay
    x: Math.round((parent.width - width) / 2)
    y: Math.round((parent.height - height) / 2)
    width: 280
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    background: Rectangle {
        color: "#1e293b"
        radius: 10
        border.color: "#ef4444"
        border.width: 1
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 15
        spacing: 15

        Text { text: "Delete Vault?"; color: "#ef4444"; font.bold: true; font.pixelSize: 15 }

        Text {
            text: "Are you sure you want to delete '" + root.vaultName + "'."
            color: "#94a3b8"
            font.pixelSize: 12
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Button {
                Layout.fillWidth: true;
                Layout.bottomMargin: 15;
                height: 35; text: "Cancel"
                contentItem: Text { text: parent.text; color: "white"; horizontalAlignment: Text.AlignHCenter }
                background: Rectangle { color: "#334155"; radius: 6 }
                onClicked: root.close()
            }

            Button {
                Layout.fillWidth: true;
                Layout.bottomMargin: 15;
                height: 35; text: "Delete"
                contentItem: Text { text: parent.text; color: "white"; font.bold: true; horizontalAlignment: Text.AlignHCenter }
                background: Rectangle { color: "#ef4444"; radius: 6 }
                onClicked: root.deleteConfirmed()
            }
        }
    }
}
// add end 20261004 Criz
