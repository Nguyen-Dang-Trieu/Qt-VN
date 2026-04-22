import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "SideBar"
import "MainContent"
import "RightPanel"

Rectangle {
    id: folderViewRoot
    color: "#111118"
    anchors.fill: parent

    signal logoutRequested()

    // --- BỌC TOÀN BỘ GIAO DIỆN VÀO SCALE WRAPPER ---
    Item {
        id: scaleWrapper
        width: 1280
        height: 720
        anchors.centerIn: parent
        // Tự động zoom to/nhỏ theo kích thước màn hình
        scale: Math.min(folderViewRoot.width / width, folderViewRoot.height / height)

        RowLayout {
            anchors.fill: parent
            spacing: 0

            // 1. CỘT TRÁI (SideBar)
            SideBar {
                id: sideBarArea
                // Đã chỉnh lại 260 đúng với kích thước thật của SideBar
                Layout.preferredWidth: 260
                Layout.fillHeight: true

                onLogoutRequested: {
                    folderViewRoot.logoutRequested()
                }

                onSelectedVaultIndexChanged: {
                    if (selectedVaultIndex !== -1) {
                        //                        var data = sideBarArea.vaultModel.get(selectedVaultIndex)
                        //                        if (data) {
                        //                            mainArea.activeVaultName = data.name
                        //                            mainArea.activeVaultPath = data.path
                        //                            mainArea.activeVaultPassword = data.password

                        //                            console.log("vault name: " + data.name);
                        //                            console.log("Path: " + data.path);
                        //                            console.log("password: " + data.password);

                        //                            vaultController.openVault(data.name, data.password, data.path);

                        //                            // Reset lại thanh điều hướng breadcrumb về Root
                        //                            mainArea.pathStack = [{"name": "Root", "id": 1}]
                        //                        }
                        //                    } else {
                        // Reset khi không chọn vault nào
                        mainArea.activeVaultName = ""
                        mainArea.activeVaultPassword = ""
                        detailPanel.selectedFileData = null
                        //                    }
                    }
                    else
                    {
                        mainArea.activeVaultName = ""
                        mainArea.activeVaultPassword = ""
                        mainArea.activeVaultPath = ""
                        mainArea.pathStack = [{"name": "Root", "id": 1}]
                        listNodes.loadFolder(0)
                    }
                }
                onVaultUnlocked: function(vName, vPath, vPassword) {
                    mainArea.activeVaultName = vName
                    mainArea.activeVaultPath = vPath
                    mainArea.activeVaultPassword = vPassword

                    // Reset thanh Breadcrumb
                    mainArea.pathStack = [{"name": "Root", "id": 1}]
                    detailPanel.selectedFileData = null
                    if (vName === "") {
                        listNodes.loadFolder(0)
                    }
                }
            }

            // 2. CỘT GIỮA (MainContent)
            MainContent {
                id: mainArea
                Layout.fillWidth: true
                Layout.fillHeight: true
                onFileSelected: function(fileData) {
                    detailPanel.selectedFileData = fileData
                }
            }

            // 3. CỘT PHẢI (RightPanel)
            RightPanel {
                id: detailPanel
                // Đã chỉnh lại 320 đúng với kích thước thật của RightPanel
                Layout.preferredWidth: 320
                Layout.fillHeight: true
            }
        }
    }
}
