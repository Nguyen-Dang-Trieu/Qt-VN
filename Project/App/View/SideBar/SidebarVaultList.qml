import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ColumnLayout {
    property color primaryBlue
    property var vaultModel
    property int selectedVaultIndex
    signal vaultSelected(int index)

    Layout.fillWidth: true
    Layout.fillHeight: true
    spacing: 12

    Text { text: "FOLDER VAULT"; font.pixelSize: 11; color: "#475569"; font.bold: true; Layout.leftMargin: 4 }

    ScrollView {
        Layout.fillWidth: true; Layout.fillHeight: true; clip: true
        ColumnLayout {
            width: parent.width; spacing: 4
            Repeater {
                model: vaultModel
                delegate: ItemDelegate {
                    Layout.fillWidth: true; height: 40
                    readonly property bool isCurrent: selectedVaultIndex === index

                    background: Rectangle {
                        color: isCurrent ? "#1e293b" : (hovered ? "#161e2f" : "transparent")
                        radius: 8
                        Rectangle {
                            width: 3; height: 16; radius: 2; color: primaryBlue
                            anchors.left: parent.left; anchors.leftMargin: 2
                            anchors.verticalCenter: parent.verticalCenter; visible: isCurrent
                        }
                    }

                    contentItem: RowLayout {
                        spacing: 12; anchors.leftMargin: 10
                        Text { text: model.icon; font.pixelSize: 14 }
                        Text { text: model.name; color: isCurrent ? "#ffffff" : "#94a3b8"; font.bold: isCurrent; font.pixelSize: 13 }
                    }
                    onClicked: vaultSelected(index)
                }
            }
        }
    }

    Text {
        text: "No vaults created"; font.pixelSize: 12; color: "#334155"
        visible: vaultModel && vaultModel.count === 0
        Layout.leftMargin: 12
    }
}
