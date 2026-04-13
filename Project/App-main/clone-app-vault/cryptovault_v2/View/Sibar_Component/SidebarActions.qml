import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ColumnLayout {
    property color primaryBlue
    property int selectedVaultIndex

    signal createClicked()
    signal renameClicked()
    signal deleteClicked()

    Layout.fillWidth: true
    Layout.topMargin: 20
    spacing: 8

    // --- Button: Create New Vault ---
    Rectangle {
        Layout.fillWidth: true; height: 38; color: primaryBlue; radius: 8
        Text { anchors.centerIn: parent; text: "+ Create New Vault"; color: "white"; font.pixelSize: 12; font.bold: true }
        MouseArea { anchors.fill: parent; cursorShape: Qt.PointingHandCursor; onClicked: createClicked() }
    }

    // --- Button: Rename + Delete ---
    RowLayout {
        Layout.fillWidth: true; spacing: 8

        Rectangle {
            Layout.fillWidth: true; height: 36; color: "#1e293b"; radius: 6
            opacity: selectedVaultIndex !== -1 ? 1.0 : 0.4
            Text { anchors.centerIn: parent; text: "Rename"; color: "#94a3b8"; font.pixelSize: 11; font.bold: true }
            MouseArea { anchors.fill: parent; enabled: selectedVaultIndex !== -1; onClicked: renameClicked() }
        }

        Rectangle {
            Layout.fillWidth: true; height: 36; color: "#450a0a"; radius: 6; border.color: "#ef4444"
            opacity: selectedVaultIndex !== -1 ? 1.0 : 0.4
            Text { anchors.centerIn: parent; text: "Delete"; color: "#ef4444"; font.pixelSize: 11; font.bold: true }
            MouseArea { anchors.fill: parent; enabled: selectedVaultIndex !== -1; onClicked: deleteClicked() }
        }
    }
}
