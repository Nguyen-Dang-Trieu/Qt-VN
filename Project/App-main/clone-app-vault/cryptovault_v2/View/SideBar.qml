import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.1

Rectangle {
    id: sidebarRoot
    Layout.fillHeight: true
    width: 260
    color: "#0f172a"
    border.color: "#1e293b"

    // --- Color ---
    readonly property color primaryBlue: "#246a99"
    readonly property color hoverBlue: "#2d82bc"
    readonly property color pressBlue: "#1b5175"

    property int selectedVaultIndex: -1             // Luu tru index cua vault dc "Click"
    property string activeMainMenu: "Overview"

    property alias vaultModel: vaultModel           // Share vs MainContet thong qua FolderTreeView

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 0

        /* ---- 1. LOGO SECTION ---- */
        RowLayout {
            Layout.bottomMargin: 30
            spacing: 12
            Rectangle {
                width: 32; height: 32; radius: 8
                color: sidebarRoot.primaryBlue
            }
            Text { text: "Drew"; font.bold: true; font.pixelSize: 20; color: "#f8fafc" }
        }

        /* ---------------------------------------------------------------------- 3. FOLDER VAULT SECTION ---- */
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 12

            // 3.1 -------------------------------------------------------------- Text
            Text { text: "FOLDER VAULT"; font.pixelSize: 11; color: "#475569"; font.bold: true; Layout.leftMargin: 4 }

            // 3.2 -------------------------------------------------------------- List Folder Vault
            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                ColumnLayout {
                    width: parent.width
                    spacing: 4
                    Repeater {
                        model: vaultModel     // Nguon data vaultModel (ListModel)
                        delegate: navDelegate // Xu li khi user click vao Name Vault
                    }
                }
            }

            Text {
                text: "No vaults created"
                font.pixelSize: 12; color: "#334155"
                visible: vaultModel.count === 0
                Layout.leftMargin: 12
            }
        }

        /* ---- 4. ACTION BUTTONS ---- */
        ColumnLayout {
            Layout.fillWidth: true
            Layout.topMargin: 20
            spacing: 8

            //  ------------------------------------------------------------------------------------- Button: "Create New Vault"
            Rectangle {
                Layout.fillWidth: true; height: 38; color: sidebarRoot.primaryBlue; radius: 8
                Text { anchors.centerIn: parent; text: "+ Create New Vault"; color: "white"; font.pixelSize: 12; font.bold: true }
                MouseArea {
                    anchors.fill: parent; cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        newVaultName.text = ""
                        newVaultPass.text = ""
                        newVaultPass.showPassword = false
                        createVaultPopup.open()
                    }
                }
            }

            //  ------------------------------------------------------------------------------------- Button: "Rename" + "Delete"
            RowLayout {
                Layout.fillWidth: true; spacing: 8

                // For "Rename"
                Rectangle {
                    Layout.fillWidth: true; height: 36; color: "#1e293b"; radius: 6
                    opacity: sidebarRoot.selectedVaultIndex !== -1 ? 1.0 : 0.4
                    Text { anchors.centerIn: parent; text: "Rename"; color: "#94a3b8"; font.pixelSize: 11; font.bold: true }
                    MouseArea {
                        anchors.fill: parent; enabled: sidebarRoot.selectedVaultIndex !== -1
                        onClicked: {
                            renameInput.text = vaultModel.get(sidebarRoot.selectedVaultIndex).name
                            renamePopup.open()
                            renameInput.forceActiveFocus()
                        }
                    }
                }

                // For "Delete"
                Rectangle {
                    Layout.fillWidth: true; height: 36; color: "#450a0a"; radius: 6; border.color: "#ef4444"
                    opacity: sidebarRoot.selectedVaultIndex !== -1 ? 1.0 : 0.4
                    Text { anchors.centerIn: parent; text: "Delete"; color: "#ef4444"; font.pixelSize: 11; font.bold: true }
                    MouseArea {
                        anchors.fill: parent; enabled: sidebarRoot.selectedVaultIndex !== -1
                        onClicked: {
                            // Lấy path từ model
                            let targetPath = vaultModel.get(sidebarRoot.selectedVaultIndex).path

                            // Gọi C++ xóa khỏi DB
                            if (vaultController.deleteVault(targetPath)) {
                                vaultModel.remove(sidebarRoot.selectedVaultIndex)
                                sidebarRoot.selectedVaultIndex = -1
                            } else {
                                console.log("Lỗi: Không thể xóa Vault khỏi DB")
                            }
                        }
                    }
                }
            }
        }
    }

    // --- POPUP: CREATE NEW VAULT (4) ---
    Popup {
        id: createVaultPopup
        parent: Overlay.overlay
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: 300; modal: true; focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside


        background: Rectangle {
            color: "#1e293b"
            radius: 12
            border.color: "#334155"
            border.width: 1
        }

        ColumnLayout {
            anchors.fill: parent;
            anchors.margins: 10;
            spacing: 18
            Text {
                text: "Create New Vault"
                color: "white";
                font.bold: true;
                font.pixelSize: 18;
                Layout.bottomMargin: 20
            }

            //  ------------------------------------------------------------------------------------- Name Vault
            ColumnLayout {
                Layout.fillWidth: true; spacing: 8
                Text { text: "Vault Name"; color: "#94a3b8"; font.pixelSize: 12; font.bold: true }

                TextField {
                    id: newVaultName
                    Layout.fillWidth: true; height: 40; color: "white"
                    placeholderText: "Enter name..."; placeholderTextColor: "#475569"
                    leftPadding: 12; font.pixelSize: 14; selectByMouse: true
                    background: Rectangle {
                        color: "#0f172a"; radius: 8
                        border.color: parent.activeFocus ? sidebarRoot.primaryBlue : "#334155"
                        border.width: parent.activeFocus ? 2 : 1
                    }
                    onAccepted: newVaultPass.forceActiveFocus()
                }
            }

            //  ------------------------------------------------------------------------------------- Password Vault
            ColumnLayout {
                Layout.fillWidth: true; spacing: 8
                Text { text: "Vault Password"; color: "#94a3b8"; font.pixelSize: 12; font.bold: true }
                TextField {
                    id: newVaultPass
                    Layout.fillWidth: true; height: 40; color: "white"

                    property bool showPassword: false
                    echoMode: showPassword ? TextInput.Normal : TextInput.Password

                    placeholderText: "••••••••"; placeholderTextColor: "#475569"
                    leftPadding: 12
                    rightPadding: 40
                    font.pixelSize: 14; selectByMouse: true

                    background: Rectangle {
                        color: "#0f172a"; radius: 8
                        border.color: parent.activeFocus ? sidebarRoot.primaryBlue : "#334155"
                        border.width: parent.activeFocus ? 2 : 1
                    }

                    // --- ICON eyes ---
                    Item {
                        anchors.right: parent.right
                        anchors.rightMargin: 12
                        anchors.verticalCenter: parent.verticalCenter
                        width: 20; height: 20

                        Text {
                            anchors.centerIn: parent
                            text: "👁"
                            font.pixelSize: 16
                            color: newVaultPass.showPassword ? sidebarRoot.primaryBlue : "#64748b"
                        }

                        // Đường gạch chéo khi ẩn
                        Rectangle {
                            anchors.centerIn: parent
                            width: 2; height: 18
                            color: "#64748b"
                            rotation: 45
                            visible: !newVaultPass.showPassword
                            antialiasing: true
                        }

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: newVaultPass.showPassword = !newVaultPass.showPassword
                        }
                    }

                    onAccepted: createBtn.clicked()
                }
            }

            //  ------------------------------------------------------------------------------------- Path Vault
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 8
                Text { text: "Storage Path"; color: "#94a3b8"; font.pixelSize: 12; font.bold: true }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 8

                    TextField {
                        id: newVaultPath
                        Layout.fillWidth: true
                        height: 40; color: "white"
                        placeholderText: "e.g. D:/MyVaults"; placeholderTextColor: "#475569"
                        leftPadding: 12; font.pixelSize: 14; selectByMouse: true
                        background: Rectangle {
                            color: "#0f172a"; radius: 8
                            border.color: parent.activeFocus ? sidebarRoot.primaryBlue : "#334155"
                            border.width: parent.activeFocus ? 2 : 1
                        }
                        onAccepted: createBtn.clicked()
                    }

                    Rectangle {
                        Layout.preferredWidth: 40
                        Layout.preferredHeight: 40
                        color: pathMouseArea.containsMouse ? sidebarRoot.hoverBlue : "#334155"
                        radius: 8

                        Text {
                            text: "📁"
                            anchors.centerIn: parent
                            font.pixelSize: 16
                        }

                        MouseArea {
                            id: pathMouseArea
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            hoverEnabled: true
                            onClicked: folderDialog.open()
                        }
                    }
                }
            }

            // ------------------------------------------------------------------------------------- BUTTON: "Confirm & Create"
            Button {
                id: createBtn
                Layout.fillWidth: true; Layout.preferredHeight: 42; Layout.topMargin: 10
                text: "Confirm & Create"
                Layout.bottomMargin: 15

                contentItem: Text { // Context of Button
                    text: parent.text; color: "white"; font.bold: true; font.pixelSize: 14
                    horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle {
                    color: parent.down ? sidebarRoot.pressBlue : (parent.hovered ? sidebarRoot.hoverBlue : sidebarRoot.primaryBlue)
                    radius: 8
                }

                onClicked: {
                    let vName = newVaultName.text.trim() // Du lieu tu vung TextField
                    let vPass = newVaultPass.text
                    let vPath = newVaultPath.text.trim()

                    if (vName !== "" && vPass !== "" && vPath !== "") {

                        // Create Dir and DB
                        let success = vaultController.createVaultFolder(vName, vPass, vPath)

                        if (success) {
                            let fullVaultPath = vPath + "/" + vName
                            // IF success, update QML
                            vaultModel.append({
                                                  name: vName,
                                                  icon: "🔐",
                                                  password: vPass,
                                                  path: fullVaultPath
                                              })

                            // Clean POPUP
                            newVaultName.text = ""
                            newVaultPass.text = ""
                            newVaultPath.text = ""
                            createVaultPopup.close()
                        } else {
                            console.log("[ERROR]: Could not create a directory for Vault.")
                        }
                    }
                }
            }
        }
    }

    // --- POPUP: RENAME ---
    Popup {
        id: renamePopup
        parent: Overlay.overlay
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: 260; modal: true; focus: true
        background: Rectangle { color: "#1e293b"; radius: 10; border.color: "#334155" }
        ColumnLayout {
            anchors.fill: parent; anchors.margins: 15; spacing: 15
            Text { text: "Rename Vault"; color: "white"; font.bold: true; font.pixelSize: 15 }
            TextField {
                id: renameInput
                Layout.fillWidth: true; color: "white"; focus: true
                background: Rectangle { color: "#0f172a"; radius: 6; border.color: parent.activeFocus ? sidebarRoot.primaryBlue : "#334155" }
            }
            Button {
                Layout.fillWidth: true;
                height: 35;
                text: "Save Changes"
                contentItem: Text { text: parent.text; color: "white"; horizontalAlignment: Text.AlignHCenter }
                background: Rectangle {
                    color: parent.hovered ? sidebarRoot.hoverBlue : sidebarRoot.primaryBlue
                    radius: 6
                }

                // 2. CHUYỂN LOGIC ĐỔI TÊN XUỐNG ĐÂY
                onClicked: {
                    let newName = renameInput.text.trim()

                    if (newName !== "") {
                        let oldPath = vaultModel.get(sidebarRoot.selectedVaultIndex).path

                        let newPath = vaultController.renameVault(oldPath, newName)

                        if (newPath !== "") {
                            vaultModel.setProperty(sidebarRoot.selectedVaultIndex, "name", newName)
                            vaultModel.setProperty(sidebarRoot.selectedVaultIndex, "path", newPath)
                            renamePopup.close()
                        } else {
                            console.log("Lỗi: Không thể đổi tên Vault (Tên trùng hoặc đang bị kẹt file)")
                        }
                    }
                }
            }
        }
    }

    // Luu tru du lieu ve all folder vault cua User (from Table User_InfoVaults in Path DB)
    ListModel {
        id: vaultModel
        Component.onCompleted: {
            vaultModel.clear()

            // Load data tu Table User_InfoVaults va save
            let dataAllVaults = vaultController.loadVaults()

            for (let i = 0; i < dataAllVaults.length; i++) {
                vaultModel.append(dataAllVaults[i]) // Get data from loadedVaults
            }
            console.log("Vault list initialization from DB complete (../SideBar.qml)")
        }
    }

    // -------------------------------------------------------------- Repeater in List Folder Vault (3.2)
    Component {
        id: navDelegate
        ItemDelegate {
            Layout.fillWidth: true; height: 40
            readonly property bool isCurrent: sidebarRoot.selectedVaultIndex === index
            background: Rectangle {
                color: isCurrent ? "#1e293b" : (hovered ? "#161e2f" : "transparent")
                radius: 8
                Rectangle {
                    width: 3; height: 16; radius: 2; color: sidebarRoot.primaryBlue
                    anchors.left: parent.left; anchors.leftMargin: 2
                    anchors.verticalCenter: parent.verticalCenter; visible: isCurrent
                }
            }

            contentItem: RowLayout { // Where data duoc lay thong qua model
                spacing: 12; anchors.leftMargin: 10
                Text { text: model.icon; font.pixelSize: 14 }
                Text { text: model.name; color: isCurrent ? "#ffffff" : "#94a3b8"; font.bold: isCurrent; font.pixelSize: 13 }
            }
            onClicked: {
                sidebarRoot.selectedVaultIndex = index
                sidebarRoot.activeMainMenu = ""

                console.log("[VIEW] Index Selected: " + index + " (../Sibar)");
            }
        }
    }

    // --- DIALOG CHỌN THƯ MỤC ---
    FolderDialog {
        id: folderDialog
        title: "Select Vault Storage Folder"
        onAccepted: {
            let path = folder.toString()

            if (Qt.platform.os === "windows") {
                path = path.replace(/^(file:\/{3})/, "") // C:/Users/...
            } else {
                path = path.replace(/^(file:\/{2})/, "") // /home/user/... (Giữ lại 1 dấu / cho Ubuntu)
            }

            // decodeURIComponent để xử lý lỗi nếu tên thư mục có chứa dấu cách
            newVaultPath.text = decodeURIComponent(path)
        }
    }
}
