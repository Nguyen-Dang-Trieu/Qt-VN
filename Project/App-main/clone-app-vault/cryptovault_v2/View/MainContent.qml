import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.1 // Need to use FolderDialog


Rectangle {
    id: mainContentRoot
    Layout.fillWidth: true
    Layout.fillHeight: true
    color: "#0f172a"

    // Luu tru thong tin cua Folder Vault dc chon de thao tac
    property string activeVaultName: ""
    property string activeVaultPath: ""
    property string activeVaultPassword: ""

    // Chua cac cap thu muc
    property var pathStack: [{"name": "Root", "id": 1}] // Khởi tạo stack với Root (ID = 1)

    signal fileSelected(var fileData)

    // Lay id cua thu muc hien tai
    function getCurrentFolderId() {
        return pathStack[pathStack.length - 1].id
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 40
        spacing: 24

        /* ---- 1. NAME VAULT ---- */
        Text {
            text: mainContentRoot.activeVaultName !== "" ? "Vault: " + mainContentRoot.activeVaultName : "Select a Vault"
            font.pixelSize: 28; font.bold: true; color: "#f8fafc"
        }

        /* ---- 2. BREADCRUMBS ---- */
        RowLayout {
            Layout.fillWidth: true
            spacing: 5

            Repeater {
                model: pathStack // save data of Repeater
                delegate: Row {
                    spacing: 5
                    Text {
                        text: modelData.name
                        color: index === pathStack.length - 1 ? "#38bdf8" : "#94a3b8"
                        font.bold: true
                        font.pixelSize: 14

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor // thay doi hinh dang con tro chuot khi di chuyen vao cung MouseArea

                            // Xu li khi click vao
                            onClicked: {
                                if (index < pathStack.length - 1) {
                                    var targetId = modelData.id;

                                    // Lay list file va folder cua theo id
                                    listNodes.loadFolder(targetId);

                                    // Cat bo cac thu muc cap con trong stack UI
                                    pathStack = pathStack.slice(0, index + 1);
                                }
                            }
                        }
                    }
                    Text {
                        text: " > "
                        color: "#475569"
                        font.pixelSize: 14
                        visible: index < pathStack.length - 1
                    }
                }
            }
        }

        /* ---- 3. VIEW IN FOLDER ---- */
        Rectangle {
            Layout.fillWidth: true; Layout.fillHeight: true
            color: "#1e293b"; radius: 12; clip: true
            border.color: "white"

            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                // 3.1 --- NAME OF COLUMNS ---
                Rectangle {
                    Layout.fillWidth: true; height: 50
                    color: "#334155"
                    border.color: "green"

                    RowLayout {
                        anchors.fill: parent; anchors.leftMargin: 20; spacing: 20
                        Text { text: "Type"; color: "#94a3b8"; font.bold: true; Layout.preferredWidth: 40 }
                        Text { text: "File Name"; color: "#94a3b8"; font.bold: true; Layout.preferredWidth: 260 }
                        Text { text: "Access"; color: "#94a3b8"; font.bold: true; Layout.preferredWidth: 150 }
                        Text { text: "Modified"; color: "#94a3b8"; font.bold: true; Layout.fillWidth: true }
                    }
                }

                // 3.2 --- List file and folder is loaded from DB by ID
                ListView {
                    id: fileListView
                    Layout.fillWidth: true; Layout.fillHeight: true
                    model: listNodes
                    delegate: fileDelegate
                    clip: true

                    // No data is stored in model (fileNodeModel)
                    Text {
                        anchors.centerIn: parent
                        text: "No items in this folder"
                        color: "#475569"
                        visible: fileListView.count === 0
                    }
                }
            }

            // DropArea để kéo thả file vào thư mục hiện tại
            DropArea {
                anchors.fill: parent
                keys: ["text/uri-list"]
                onDropped: {
                               if (drop.hasUrls) {
                                   if (activeVaultName === "" || activeVaultPassword === "") {
                                       console.log("[VIEW] Error: Vault not selected or password missing!")
                                       return;
                                   }

                                   for (var i = 0; i < drop.urls.length; i++) {
                                       let rawPath = drop.urls[i].toString()
                                       // Gọi backend lưu vào DB dưới parentId hiện tại
                                       let success = vaultController.importDataToVault(rawPath, activeVaultName, activeVaultPassword)
                                       if (success) {
                                           // Refresh lại model để hiển thị file vừa thêm
                                           listNodes.loadFolder(getCurrentFolderId())
                                       }
                                   }
                               }
                           }
            }
        }
    }


    // --- delegate of ListView (Section 3.2) ---
    Component {
        id: fileDelegate
        Rectangle {
            width: fileListView.width; height: 60
            color: (typeof isSelected !== "undefined" && isSelected) ? "#334155" : (mouseArea.containsMouse ? "#1e293b" : "transparent")

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                hoverEnabled: true

                onClicked: {
                    // Logic chọn file (Signal ra ngoài)
                    mainContentRoot.fileSelected({"name": nodeName, "id": nodeId})
                }

                onDoubleClicked: {
                    if (nodeType === 1) { // Dir
                        // 1. Update Breadcrumb UI
                        pathStack = [...pathStack, {"name": nodeName, "id": nodeId}]

                        // 2. Load data from DB
                        listNodes.loadFolder(nodeId)
                    } else { // LÀ FILE THÌ MỞ BẢNG XÁC NHẬN
                        exportConfirmDialog.fileId = nodeId
                        exportConfirmDialog.fileName = nodeName
                        exportPathField.text = ""
                        exportConfirmDialog.open()
                    }
                }
            }

            RowLayout {
                anchors.fill: parent; anchors.leftMargin: 20; spacing: 20
                // nodeType, nodeName, nodeSize lấy từ Roles trong fileNodeModel.cpp/h
                Text { text: nodeType === 1 ? "📁" : "📄"; font.pixelSize: 18; Layout.preferredWidth: 40 }
                Text { text: nodeName; color: "#f1f5f9"; Layout.preferredWidth: 260; elide: Text.ElideRight }
                Text { text: "Only you"; color: "#94a3b8"; Layout.preferredWidth: 150 }
                Text {
                    text: nodeType === 1 ? "--" : (nodeSize + " B")
                    color: "#94a3b8"; Layout.fillWidth: true
                }
            }

            Rectangle {
                anchors.bottom: parent.bottom; width: parent.width; height: 1
                color: "#334155"
            }
        }
    }

    FolderDialog {
        id: folderDialog
        title: "Select Destination Folder"
        onAccepted: {
            // Chuyển từ URL (file:///D:/...) sang Path (D:/...)
            let path = folder.toString().replace("file:///", "")
            // Nếu trên Windows, đôi khi cần xử lý thêm dấu /
            exportPathField.text = path
        }
    }

    Popup { // ----------------------------------------- Popup cho SAVE AS
        id: exportConfirmDialog
        anchors.centerIn: parent
        width: 480
        modal: true
        focus: true
        padding: 0

        property int fileId: -1
        property string fileName: ""

        background: Rectangle {
            color: "#1e293b"
            radius: 16
            border.color: "#334155"
        }

        contentItem: ColumnLayout {
            anchors.fill: parent
            anchors.margins: 24
            spacing: 20

            // ---- TITLE ----
            ColumnLayout {
                spacing: 6
                Text {
                    text: "Export File"
                    font.pixelSize: 20
                    font.bold: true
                    color: "#f1f5f9"
                }
                Text {
                    text: exportConfirmDialog.fileName
                    font.pixelSize: 14
                    color: "#94a3b8"
                }
            }

            // ---- PATH INPUT ----
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 8

                Text {
                    text: "Destination"
                    color: "#94a3b8"
                    font.pixelSize: 12
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10

                    TextField {
                        id: exportPathField
                        Layout.fillWidth: true
                        height: 40
/*
  placeholderText: chỉ xuất hiện khi thuộc tính text = "" (rỗng). Đóng vai trò nhắc nhở user.
  Text là nội dung thực sự của ô nhập dữ liệu

*/
                        placeholderText: "Select folder..."
                        color: "#e2e8f0"

                        background: Rectangle {
                            color: "#0f172a"
                            radius: 10
                            border.color: "#334155"
                        }
                    }

                    // Button icon
                    Rectangle {
                        width: 44
                        height: 40
                        radius: 10
                        color: mouseArea.containsMouse ? "#334155" : "#1e293b"
                        border.color: "#475569"

                        Text {
                            anchors.centerIn: parent
                            text: "📁"
                            font.pixelSize: 16
                        }

                        MouseArea {
                            id: mouseArea
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: folderDialog.open()
                        }
                    }
                }
            }

            // ---- ACTION BUTTONS OF POPUP ----
            RowLayout {
                Layout.fillWidth: true
                Layout.topMargin: 10
                spacing: 10

                Item { Layout.fillWidth: true } // push buttons to right

                // Cancel
                Rectangle {
                    width: 100
                    height: 40
                    radius: 10
                    color: mouseCancel.containsMouse ? "#334155" : "#1e293b"
                    border.color: "#475569"

                    Text {
                        anchors.centerIn: parent
                        text: "Cancel"
                        color: "#e2e8f0"
                    }

                    MouseArea {
                        id: mouseCancel
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: exportConfirmDialog.close()
                    }
                }

                // Confirm (Primary button)
                Rectangle {
                    width: 120
                    height: 40
                    radius: 10
                    color: mouseOk.containsMouse ? "#0284c7" : "#0ea5e9"

                    Text {
                        anchors.centerIn: parent
                        text: "Export"
                        color: "white"
                        font.bold: true
                    }

                    MouseArea {
                        id: mouseOk
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor

                        onClicked: {
                            vaultController.exportFileFromVault(
                                exportConfirmDialog.fileId,     // ID của file được clicked
                                exportPathField.text,           // text là đường dẫn đến thư mục mà user cần SAVE AS
                                exportConfirmDialog.fileName    // Name của file được click
                            )
                            exportConfirmDialog.close()   // ĐÓNG POPUP sau khi xử lí xong
                        }
                    }
                }
            }
        }
    }
}
