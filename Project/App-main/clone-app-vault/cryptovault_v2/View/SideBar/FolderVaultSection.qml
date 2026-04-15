// File: FolderVaultSection.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ColumnLayout {
    id: vaultSectionRoot
    Layout.fillWidth: true
    Layout.fillHeight: true
    spacing: 12

    // --- 1. NHẬN DỮ LIỆU TỪ COMPONENT CHA ---
    property var vaultModel // Nhận ListModel từ SideBar truyền xuống
    property int selectedIndex: -1
    property color primaryColor: "#0284c7"

    // --- 2. KHAI BÁO TÍN HIỆU ---
    signal vaultClicked(int clickedIndex)

    // --- 3. GIAO DIỆN ---
    Text {
        text: "FOLDER VAULT"
        font.pixelSize: 11
        color: "#475569"
        font.bold: true
        Layout.leftMargin: 4
    }

    ScrollView {
        id: vaultScrollView
        Layout.fillWidth: true
        Layout.fillHeight: true
        clip: true

        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

        ColumnLayout {
            id: vaultListLayout
            width: vaultScrollView.availableWidth
            spacing: 4

            Repeater {
                model: vaultSectionRoot.vaultModel

                delegate: NavDelegate {
                    Layout.fillWidth: true
                    selectedIndex: vaultSectionRoot.selectedIndex
                    primaryColor: vaultSectionRoot.primaryColor

                    // Bắt tín hiệu từ file NavDelegate.qml và đẩy tiếp lên cha
                    onVaultClicked: function(index) {
                        vaultSectionRoot.vaultClicked(index)
                    }
                }
            }
        }
    }

    Text {
        text: "No vaults created"
        font.pixelSize: 12
        color: "#334155"
        // Chỉ hiển thị khi model tồn tại và số lượng vault = 0
        visible: vaultSectionRoot.vaultModel ? vaultSectionRoot.vaultModel.count === 0 : false
        Layout.leftMargin: 12
    }
}
