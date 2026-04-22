import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.1
import "SideBarLogic.js" as Logic
import "../LogOut"
import "Popup"

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

    signal logoutRequested()
    signal vaultUnlocked(string vaultName, string vaultPath, string vaultPassword)

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // --- 1. LOGO SECTION ---
        RowLayout {
            Layout.margins: 12
            Layout.bottomMargin: 20
            spacing: 12

            // Gói Logo + Tên vào 1 khu vực để bắt sự kiện chuột
            Item {
                Layout.preferredWidth: 32 + 12 + userNameText.contentWidth
                Layout.preferredHeight: 32

                RowLayout {
                    anchors.fill: parent
                    spacing: 12
                    Rectangle {
                        id: logoIcon
                        width: 32; height: 32; radius: 8
                        color: sidebarRoot.primaryBlue
                    }
                    Text {
                        id: userNameText
                        text: authManager.getActiveUsername();
                        font.bold: true; font.pixelSize: 20; color: "#f8fafc"
                    }
                }

                // Vùng bắt sự kiện Chuột Phải
                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.RightButton
                    cursorShape: Qt.PointingHandCursor
                    onClicked: function(mouse) {
                        // Gọi hàm show menu ở vị trí con trỏ chuột
                        sidebarRoot.showAccountContextMenu(mouse.x + parent.x, mouse.y + parent.y)
                    }
                }
            }


            // Nút Log Out gọi từ file LogOut.qml
//            LogOut {
//                onLogoutClicked: {
//                    sidebarRoot.logoutRequested()
//                }
//            }
        }

        // --- 2. ACTIVE VAULT INDICATOR (NEW) ---
        Rectangle {
            Layout.fillWidth: true
            Layout.margins: 12
            Layout.bottomMargin: 20
            Layout.preferredHeight: 64
            radius: 8

            // Đổi màu nền thẻ tùy thuộc vào việc có chọn Vault hay chưa
            color: sidebarRoot.selectedVaultIndex !== -1 ? "#161B26" : "transparent"
            border.color: sidebarRoot.selectedVaultIndex !== -1 ? "#1A2535" : "transparent"
            border.width: 1

            RowLayout {
                anchors.fill: parent
                anchors.margins: 12
                spacing: 12

                // Khối thông tin chữ
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 4

                    Text {
                        text: "SELECTED VAULT FOLDER"
                        color: sidebarRoot.selectedVaultIndex !== -1 ? sidebarRoot.hoverBlue : "#5E6A7A"
                        font.pixelSize: 10
                        font.bold: true
                        font.letterSpacing: 1.2
                    }

                    Text {
                        // Tự động lấy tên Vault từ ListModel dựa vào index đang chọn
                        text: {
                            if (sidebarRoot.selectedVaultIndex !== -1 && vaultModel.count > sidebarRoot.selectedVaultIndex) {
                                // Lấy thuộc tính name (hoặc vaultName tùy theo cách bạn đặt trong loadedVaults)
                                return vaultModel.get(sidebarRoot.selectedVaultIndex).name || vaultModel.get(sidebarRoot.selectedVaultIndex).vaultName || "Unknown Vault"
                            }
                            return "No vault selected"
                        }

                        color: sidebarRoot.selectedVaultIndex !== -1 ? "#FFFFFF" : "#475569"
                        font.pixelSize: 14
                        font.bold: true
                        elide: Text.ElideRight // Cắt chữ hiển thị dấu "..." nếu tên quá dài
                        Layout.fillWidth: true
                    }
                }
            }
        }
        // --- 3. FOLDER VAULT SECTION ---
        FolderVaultSection {
            // Truyền dữ liệu xuống
            vaultModel: sidebarRoot.vaultModel
            selectedIndex: sidebarRoot.selectedVaultIndex
            primaryColor: sidebarRoot.primaryBlue
            Layout.fillWidth: true
            Layout.margins: 12

            // Hứng tín hiệu click từ con
            onVaultClicked: function(clickedIndex) {
                sidebarRoot.selectedVaultIndex = clickedIndex
                sidebarRoot.activeMainMenu = ""
                console.log("Sidebar: Đã chọn index " + clickedIndex)

                // Nếu click ra vùng trống thì clear màn hình
                if (clickedIndex === -1) {
                    sidebarRoot.vaultUnlocked("", "", "")
                    return;
                }

                let data = vaultModel.get(clickedIndex)

                // KIỂM TRA TRẠNG THÁI: Nếu đã có password tức là ĐÃ UNLOCK
                if (data && data.password && data.password !== "") {
                    // Mở file trực tiếp không cần hỏi Popup
                    vaultController.openVault(data.name, data.password, data.path)
                    sidebarRoot.vaultUnlocked(data.name, data.path, data.password)
                } else {
                    // CHƯA UNLOCK: Gửi data rỗng để MainContent ẩn danh sách file đi
                    sidebarRoot.vaultUnlocked("", "", "")
                }
            }
            onVaultDoubleClicked: function(clickedIndex) {
                sidebarRoot.selectedVaultIndex = clickedIndex;

                if (clickedIndex === -1) return;

                let data = vaultModel.get(clickedIndex)

                // Kể cả khi Double-click, nếu đã unlock rồi thì cũng mở file luôn cho mượt
                if (data && data.password && data.password !== "") {
                    vaultController.openVault(data.name, data.password, data.path)
                    sidebarRoot.vaultUnlocked(data.name, data.path, data.password)
                } else {
                    // CHƯA UNLOCK: Hiển thị Popup bắt nhập mật khẩu
                    Logic.openDynamicUnlockPopup(sidebarRoot, vaultController, clickedIndex);
                }
            }

            onChangeVaultPasswordRequested: Logic.openDynamicChangePasswordPopup(sidebarRoot, vaultController)
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
            onImportClicked: Logic.openDynamicImportPopup(sidebarRoot, vaultController) // add 20260414
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

    // Hàm trung gian để hiện Menu tại vị trí chuột
    function showVaultContextMenu(mx, my, idx) {
        vaultMenu.x = mx // Lệch một chút cho đẹp
        vaultMenu.y = my
        vaultMenu.targetIndex = idx
        vaultMenu.open()
    }

    function showAccountContextMenu(mx, my) {
        accountMenu.x = mx
        accountMenu.y = my
        accountMenu.open()
    }

    VaultMenuPopup {
        id: vaultMenu
        onChangePasswordRequested: function(idx) {
            Logic.openDynamicChangeVaultPasswordPopup(sidebarRoot, vaultController)
        }

        onForgetPasswordRequested: function(idx) {
            console.log("Đã bấm Forgot Password cho Vault số: ", idx)
            Logic.openDynamicForgetPasswordPopup(sidebarRoot, vaultController)
        }
    }

    AccountMenuPopup {
        id: accountMenu
        onChangeAccountPasswordRequested: function() {
            Logic.openDynamicChangeAccountPasswordPopup(sidebarRoot, authManager)
        }
        onLogoutRequested: function() {
            console.log("Đang xử lý đăng xuất...")
            sidebarRoot.logoutRequested()
        }
    }
}


