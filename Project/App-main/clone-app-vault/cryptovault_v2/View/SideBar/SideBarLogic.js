// File: SideBarLogic.js

function openDynamicRenamePopup(sidebarRoot, vaultController) {
    if (sidebarRoot.selectedVaultIndex === -1) return

    // Lấy model thông qua sidebarRoot
    let currentName = sidebarRoot.vaultModel.get(sidebarRoot.selectedVaultIndex).name

    let comp = Qt.createComponent("qrc:/View/SideBar/Popup/RenamePopup.qml")
    console.log("DEBUG: Trạng thái Component Rename:", comp.status);

    if (comp.status === Component.Ready){
        let popup = comp.createObject(sidebarRoot, {
            "currentName": currentName,
            "primaryBlue": sidebarRoot.primaryBlue,
            "hoverBlue": sidebarRoot.hoverBlue
        })

        popup.renameConfirmed.connect(function(newName) {
            let oldPath = sidebarRoot.vaultModel.get(sidebarRoot.selectedVaultIndex).path
            // Gọi C++ logic
            let newPath = vaultController.renameVault(oldPath, newName)

            if (newPath !== "") {
                sidebarRoot.vaultModel.setProperty(sidebarRoot.selectedVaultIndex, "name", newName)
                sidebarRoot.vaultModel.setProperty(sidebarRoot.selectedVaultIndex, "path", newPath)
                popup.close()
            } else {
                console.log("Lỗi: Không thể đổi tên Vault")
            }
        })

        popup.closed.connect(function() { popup.destroy() })
        popup.open()
    } else {
        console.error("Lỗi tải RenamePopup:", comp.errorString())
    }
}

function openDynamicDeletePopup(sidebarRoot, vaultController) {
    if (sidebarRoot.selectedVaultIndex === -1) return

    let vName = sidebarRoot.vaultModel.get(sidebarRoot.selectedVaultIndex).name
    let comp = Qt.createComponent("qrc:/View/SideBar/Popup/DeletePopup.qml")

    console.log("DEBUG: Trạng thái Delete Popup:", comp.status);

    if (comp.status === Component.Ready) {
        let popup = comp.createObject(sidebarRoot, { "vaultName": vName })

        popup.deleteConfirmed.connect(function() {
            let targetPath = sidebarRoot.vaultModel.get(sidebarRoot.selectedVaultIndex).path
            if (vaultController.deleteVault(targetPath)) {
                sidebarRoot.vaultModel.remove(sidebarRoot.selectedVaultIndex)
                sidebarRoot.selectedVaultIndex = -1
                popup.close()
            } else {
                console.log("Lỗi: Không thể xóa Vault")
            }
        })

        popup.closed.connect(function() { popup.destroy() })
        popup.open()
    } else {
        console.error("Lỗi tải DeletePopup:", comp.errorString())
    }
}

function openDynamicCreatePopup(sidebarRoot, vaultController) {
    let comp = Qt.createComponent("qrc:/View/SideBar/Popup/CreateVaultPopup.qml")

    if (comp.status === Component.Ready) {
        let popup = comp.createObject(sidebarRoot, {
            "primaryBlue": sidebarRoot.primaryBlue,
            "hoverBlue": sidebarRoot.hoverBlue,
            "pressBlue": sidebarRoot.pressBlue
        })

        popup.createConfirmed.connect(function(vName, vPass, vPath) {
            let success = vaultController.createVaultFolder(vName, vPass, vPath)

            if (success) {
                let fullVaultPath = vPath + "/" + vName
                sidebarRoot.vaultModel.append({
                    name: vName,
                    icon: "🔐",
                    password: vPass,
                    path: fullVaultPath
                })
                popup.close()
            } else {
                console.log("[ERROR]: Could not create a directory for Vault.")
            }
        })

        popup.closed.connect(function() { popup.destroy() })
        popup.open()
    } else {
        console.error("Lỗi tải CreateVaultPopup:", comp.errorString())
    }
}
