import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.1
import "SideBarLogic.js" as Logic

Rectangle {
    id: sidebarRoot
    Layout.fillHeight: true
    width: 260

    color: "#111118"
    border.color: "#1a1a2e"

    // --- COLOR ACCENT (NEON CYAN) ---
    readonly property color primaryBlue: "#0284c7"
    readonly property color hoverBlue: "#0ea5e9"
    readonly property color pressBlue: "#0369a1"

    property int selectedVaultIndex: -1
    property string activeMainMenu: "Overview"

    property alias vaultModel: vaultModel

//    // ==========================================
//    MouseArea {
//        anchors.fill: parent
//        // Đặt z thấp để chắc chắn nó nằm dưới cùng, không đè lên các nút khác
//        z: -1
//        onClicked: {
//            // Khi click vào vùng trống, reset lại index
//            sidebarRoot.selectedVaultIndex = -1
//        }
//    }
//   // ==========================================

    ColumnLayout {
        anchors.fill: parent
        //anchors.margins: 12
        spacing: 0

        // --- 1. LOGO SECTION ---
        RowLayout {
            Layout.margins: 12
            Layout.bottomMargin: 30
            spacing: 12
            Rectangle {
                width: 32; height: 32; radius: 8
                color: sidebarRoot.primaryBlue
            }
            Text { text: "Drew"; font.bold: true; font.pixelSize: 20; color: "#f8fafc" }
        }

        // --- 2. GENERAL SECTION ---
//        GeneralSection {
//            Layout.margins: 12
//            // Truyền dữ liệu xuống
//            activeMenu: sidebarRoot.activeMainMenu
//            primaryColor: sidebarRoot.primaryBlue

//            // Hứng tín hiệu click từ con
//            onMenuClicked: function(menuName) {
//                sidebarRoot.activeMainMenu = menuName
//                sidebarRoot.selectedVaultIndex = -1 // Bỏ chọn Vault khi bấm vào mục General
//            }
//        }

        // --- 3. FOLDER VAULT SECTION ---
        FolderVaultSection {
            // Truyền dữ liệu xuống
            vaultModel: sidebarRoot.vaultModel
            selectedIndex: sidebarRoot.selectedVaultIndex
            primaryColor: sidebarRoot.primaryBlue

            // Hứng tín hiệu click từ FolderVaultSection.qml
            onVaultClicked: function(clickedIndex) {
                sidebarRoot.selectedVaultIndex = clickedIndex
                sidebarRoot.activeMainMenu = ""
                console.log("[VIEW] Selected Index " + clickedIndex + " (../SideBar.qml)")
            }
        }

        // --- 4. ACTION BUTTONS ---
        ActionButtons {
            Layout.fillWidth: true
            Layout.margins: 12
            Layout.topMargin: 20
            spacing: 10

            // Truyền dữ liệu xuống cho ActionButtons
            primaryColor: sidebarRoot.primaryBlue
            selectedIndex: sidebarRoot.selectedVaultIndex

            // Bắt tín hiệu và gọi logic tương ứng
            onCreateClicked: Logic.openDynamicCreatePopup(sidebarRoot, vaultController)
            onRenameClicked: Logic.openDynamicRenamePopup(sidebarRoot, vaultController)
            onDeleteClicked: Logic.openDynamicDeletePopup(sidebarRoot, vaultController)
        }
    }

    // Save Data
    ListModel {
        id: vaultModel
        Component.onCompleted: {
            vaultModel.clear()

            // Gọi hàm C++ (Hàm này đã được nối sang class PathDB)
            let loadedVaults = vaultController.loadVaults()

            for (let i = 0; i < loadedVaults.length; i++) {
                vaultModel.append(loadedVaults[i])
            }
            console.log("Đã khởi tạo xong danh sách Vault từ DB.")
        }
    }
}
