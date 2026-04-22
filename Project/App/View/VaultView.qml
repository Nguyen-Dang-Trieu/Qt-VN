import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

RowLayout {
    id: vaultRoot
    anchors.fill: parent
    spacing: 0

    // --- Signal ---
    signal logout()

    // Sidebar bên trái
    Rectangle {
        Layout.preferredWidth: 250; Layout.fillHeight: true
        color: "#0D1117"

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 10

            ListView {
                Layout.fillWidth: true; Layout.fillHeight: true
                model: ["My Vault", "Work Files"]
                delegate: ItemDelegate {
                    text: modelData
                    width: parent.width
                }
            }

            Button {
                text: "Logout"
                Layout.fillWidth: true
                onClicked: vaultRoot.logout() // PHÁT SIGNAL
            }
        }
    }

    // Vùng nội dung chính (Bảng danh sách file)
    Rectangle {
        Layout.fillWidth: true; Layout.fillHeight: true
        color: Style.bgSecondary

        ColumnLayout {
            anchors.fill: parent; anchors.margins: 20

            // Header của bảng
            RowLayout {
                Layout.fillWidth: true
                Label { text: "NAME"; Layout.fillWidth: true; color: Style.textSecondary }
                Label { text: "SIZE"; Layout.preferredWidth: 100; color: Style.textSecondary }
                Label { text: "DATE"; Layout.preferredWidth: 150; color: Style.textSecondary }
            }

            // Danh sách file
            ListView {
                Layout.fillWidth: true; Layout.fillHeight: true
                model: ListModel {
                    ListElement { name: "Passports_Scan.pdf"; size: "1.2 MB"; date: "Today" }
                    ListElement { name: "Seed_Phases.txt"; size: "4 KB"; date: "Yesterday" }
                }
                delegate: Rectangle {
                    width: parent.width; height: 50; color: "transparent"
                    RowLayout {
                        anchors.fill: parent
                        Label { text: name; Layout.fillWidth: true; color: "white" }
                        Label { text: size; Layout.preferredWidth: 100; color: Style.textSecondary }
                        Label { text: date; Layout.preferredWidth: 150; color: Style.textSecondary }
                        Button { text: "⋮"; flat: true }
                    }
                    Rectangle { height: 1; width: parent.width; color: "#30363D"; anchors.bottom: parent.bottom }
                }
            }
        }
    }
}
