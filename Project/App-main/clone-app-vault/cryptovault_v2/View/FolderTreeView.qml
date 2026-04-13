import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: folderViewRoot
    color: "#0f172a"
    anchors.fill: parent

    RowLayout {
        anchors.fill: parent
        spacing: 0
        SideBar {
            id: sideBarArea

            // Khi selectedVaultIndex thay doi, QML call:
            onSelectedVaultIndexChanged: {
                if (selectedVaultIndex !== -1) {
                    var data = sideBarArea.vaultModel.get(selectedVaultIndex)
                    if (data) {
                        // 1. Truyền tên Vault
                        mainArea.activeVaultName     = data.name
                        mainArea.activeVaultPath     = data.path
                        mainArea.activeVaultPassword = data.password
                        mainArea.clearFiles()

                        // console.log("[VIEW] FolderView: Đã đồng bộ Vault " + data.name + " và Password sang MainContent")
                    }
                } else {
                    // Reset khi không chọn vault nào
                    mainArea.activeVaultName = ""
                    mainArea.activeVaultPassword = ""
                }
            }
        }

        MainContent {
            id: mainArea
        }

        RightPanel {
            id: detailPanel
        }
    }
}
