import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "Sibar_component"

Rectangle {
    id: sidebarRoot
    Layout.fillHeight: true
    width: 260
    color: "#0f172a"
    border.color: "red" // "#1e293b"

    // --- Color ---
    readonly property color primaryBlue: "#246a99"
    readonly property color hoverBlue: "#2d82bc"
    readonly property color pressBlue: "#1b5175"

    property int selectedVaultIndex: -1
    property string activeMainMenu: "Overview"

    property alias vaultModel: vaultModel
    property string activeVaultPassword: ""

    // Luu tru du lieu
    ListModel { id: vaultModel }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 0

        // --- 1. LOGO SECTION ---
        SidebarLogo {
            primaryBlue: sidebarRoot.primaryBlue
        }

        // --- 2. GENERAL SECTION ---
        SidebarGeneral {
            primaryBlue: sidebarRoot.primaryBlue
            activeMainMenu: sidebarRoot.activeMainMenu
            onMenuSelected: (menuName) => {
                                sidebarRoot.activeMainMenu = menuName
                                sidebarRoot.selectedVaultIndex = -1
                            }
        }

        // --- 3. FOLDER VAULT SECTION ---
        SidebarVaultList {
            primaryBlue: sidebarRoot.primaryBlue
            vaultModel: sidebarRoot.vaultModel
            selectedVaultIndex: sidebarRoot.selectedVaultIndex
            onVaultSelected: (index) => {
                                 sidebarRoot.selectedVaultIndex = index
                                 sidebarRoot.activeMainMenu = ""

                                 // LẤY MẬT KHẨU TỪ MODEL KHI CHỌN
                                 let vaultData = vaultModel.get(index)
                                 sidebarRoot.activeVaultPassword = vaultData.password

                                 // Thông báo cho MainContent biết (Giả sử bạn có biến liên kết ở file cha)
                                 // Ví dụ: mainContent.activeVaultPassword = vaultData.password
                                 console.log("[VIEW] Sidebar: Đã chọn Vault:", vaultData.name)
                             }
        }

        // --- 4. ACTION BUTTONS ---
        SidebarActions {
            primaryBlue: sidebarRoot.primaryBlue
            selectedVaultIndex: sidebarRoot.selectedVaultIndex

            onCreateClicked: createVaultPopup.openPopup()
            onRenameClicked: renamePopup.openPopup(vaultModel.get(sidebarRoot.selectedVaultIndex).name)
            onDeleteClicked: {
                vaultModel.remove(sidebarRoot.selectedVaultIndex)
                sidebarRoot.selectedVaultIndex = -1
            }
        }
    }

    // --- POPUP: CREATE NEW VAULT ---
    SidebarCreatePopup {
        id: createVaultPopup
        primaryBlue: sidebarRoot.primaryBlue
        hoverBlue: sidebarRoot.hoverBlue
        pressBlue: sidebarRoot.pressBlue
        vaultModel: sidebarRoot.vaultModel
    }

    // --- POPUP: RENAME ---
    SidebarRenamePopup {
        id: renamePopup
        primaryBlue: sidebarRoot.primaryBlue
        hoverBlue: sidebarRoot.hoverBlue
        vaultModel: sidebarRoot.vaultModel
        selectedVaultIndex: sidebarRoot.selectedVaultIndex
    }
}
