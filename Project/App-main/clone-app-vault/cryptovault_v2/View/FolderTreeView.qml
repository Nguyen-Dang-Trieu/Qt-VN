import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "SideBar"
import "MainContent"
import "RightPanel"
Rectangle {
    id: folderViewRoot
    color: "#0f172a"
    anchors.fill: parent

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // 1. CỘT TRÁI (SideBar) - Kích thước cố định
        SideBar {
            id: sideBarArea

            // BỔ SUNG: Ép chiều rộng và cho phép dãn hết chiều cao
            Layout.preferredWidth: 350
            Layout.fillHeight: true

            onSelectedVaultIndexChanged: {
                if (selectedVaultIndex !== -1) {
                    var data = sideBarArea.vaultModel.get(selectedVaultIndex)
                    if (data) {
                        mainArea.activeVaultName     = data.name
                        mainArea.activeVaultPath     = data.path
                        mainArea.activeVaultPassword = data.password

                        // mainArea.clearFiles()
                        // console.log("[VIEW] FolderView: Đã đồng bộ Vault " + data.name + " và Password sang MainContent")
                        console.log("vault name: " + data.name);
                        console.log("Path: " + data.path);
                        console.log("password: " + data.password);

                        vaultController.openVault(data.name, data.password, data.path);

                        // 3. (Tùy chọn) Reset lại thanh điều hướng breadcrumb về Root
                        mainArea.pathStack = [{"name": "Root", "id": 1}]

                    }
                } else {
                    // Reset khi không chọn vault nào
                    mainArea.activeVaultName = ""
                    mainArea.activeVaultPassword = ""
                }
            }
        }

        // 2. CỘT GIỮA (MainContent) - Co giãn linh hoạt
        MainContent {
            id: mainArea

            // BỔ SUNG: Đây là chìa khóa! Lệnh này ép MainContent chiếm TOÀN BỘ khoảng trống còn lại của màn hình
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        // 3. CỘT PHẢI (RightPanel) - Kích thước cố định (Bạn chỉnh số 300 lại theo thực tế nhé)
        RightPanel {
            id: detailPanel
            Layout.preferredWidth: 400
            Layout.fillHeight: true
        }
    }
}
