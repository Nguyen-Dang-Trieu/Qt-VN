import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.1
import "Popup"

Rectangle {
    id: mainContentRoot
    Layout.fillWidth: true
    Layout.fillHeight: true
    color: "#060913"

    // Luu tru thong tin cua Folder Vault dc chon de thao tac
    property string activeVaultName: ""
    property string activeVaultPath: ""
    property string activeVaultPassword: ""

    // --- LOGIC ĐIỀU HƯỚNG ---
    // Khởi tạo stack với Root (ID = 1)
    property var pathStack: [{"name": "Root", "id": 1}] // Khởi tạo stack với Root (ID = 1)

    signal fileSelected(var fileData)

    // Lay id cua thu muc hien tai
    function getCurrentFolderId() {
        return pathStack[pathStack.length - 1].id
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 5
        spacing: 24

        /* ---- 1. NAME VAULT ---- */
        Text {
            text: mainContentRoot.activeVaultName !== "" ? "Vault: " + mainContentRoot.activeVaultName : "Select a Vault"
            font.pixelSize: 28; font.bold: true; color: "white"
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
                        // Nếu là mục cuối cùng (thư mục đang đứng) thì màu xanh sáng, ngược lại màu xám
                        color: index === pathStack.length - 1 ? "#4EA8FE" : "#8E9AAB"
                        font.bold: true
                        font.pixelSize: 14

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor // thay doi hinh dang con tro chuot khi di chuyen vao cung MouseArea

                            // Xu li khi click vao
                            onClicked: {
                                if (index < pathStack.length - 1) {
                                    var targetId = modelData.id;
                                    // Gọi C++ load dữ liệu mới từ DB
                                    listNodes.loadFolder(targetId);
                                    // Cắt bỏ các thư mục cấp con trong stack UI
                                    pathStack = pathStack.slice(0, index + 1);
                                }
                            }
                        }
                    }
                    Text {
                        text: " > "
                        color: "#5E6A7A"
                        font.pixelSize: 14
                        visible: index < pathStack.length - 1
                    }
                }
            }
        }

        /* ---- 3. VIEW IN FOLDER ---- */
        Rectangle {
            Layout.fillWidth: true;
            Layout.fillHeight: true
            color: "#0D1321"
            radius: 8
            clip: true
            border.color: "#1A2535"
            border.width: 1
            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                // 3.1 --- NAME OF COLUMNS ---
                Rectangle {
                    Layout.fillWidth: true; height: 50
                    color: "#121A2F"

                    // viền dưới ngăn cách header và list
                    Rectangle {
                        width: parent.wdth
                        height: 1
                        color: "#1A2535"
                        anchors.bottom: parent.bottom
                    }

                    RowLayout {
                        anchors.fill: parent;
                        anchors.leftMargin: 20;
                        spacing: 20
                        // Áp dụng style chữ của nhãn (Label) bên trang Đăng ký
                        Text { text: "TYPE"; color: "#8E9AAB"; font.bold: true; font.pixelSize: 11; font.letterSpacing: 1; Layout.preferredWidth: 40 }
                        Text { text: "FILE NAME"; color: "#8E9AAB"; font.bold: true; font.pixelSize: 11; font.letterSpacing: 1; Layout.preferredWidth: 260 }
                        Text { text: "ACCESS"; color: "#8E9AAB"; font.bold: true; font.pixelSize: 11; font.letterSpacing: 1; Layout.preferredWidth: 150 }
                        Text { text: "MODIFIED"; color: "#8E9AAB"; font.bold: true; font.pixelSize: 11; font.letterSpacing: 1; Layout.fillWidth: true }
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
                        color: "#5E6A7A"
                        font.pixelSize: 13
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
                acceptedButtons: Qt.RightButton | Qt.LeftButton
                onClicked: function(mouse) {
                    // Logic chọn file (Signal hiển thị sang RightPanel)
                    mainContentRoot.fileSelected({"name": nodeName, "id": nodeId})

                    // Xử lý riêng khi click CHUỘT PHẢI
                    if (mouse.button === Qt.RightButton) {
                        console.log("[DEBUG] Đã nhận click chuột phải, mở Menu cho: " + nodeName)

                        // Gán thông tin file vào Popup
                        fileContextMenu.targetNodeId = nodeId
                        fileContextMenu.targetNodeName = nodeName
                        fileContextMenu.targetNodeType = nodeType

                        // SỬA Ở ĐÂY: Chuyển đổi tọa độ chuột từ MouseArea ra tọa độ của mainContentRoot
                        let mappedPos = mouseArea.mapToItem(mainContentRoot, mouse.x, mouse.y)

                        // Đặt tọa độ và mở Popup
                        fileContextMenu.x = mappedPos.x
                        fileContextMenu.y = mappedPos.y
                        fileContextMenu.open()
                    }
                }

                onDoubleClicked: {
                    if (nodeType === 1) { // Dir
                        // 1. Update Breadcrumb UI
                        pathStack = [...pathStack, {"name": nodeName, "id": nodeId}]

                        // 2. Load data from DB
                        listNodes.loadFolder(nodeId)
                    }
//                  else { // LÀ FILE THÌ MỞ BẢNG XÁC NHẬN
//                        exportConfirmDialog.fileId = nodeId
//                        exportConfirmDialog.fileName = nodeName
//                        exportPathField.text = ""
//                        exportConfirmDialog.open()
//                    }
                }
            }

            RowLayout {
                anchors.fill: parent; anchors.leftMargin: 20; spacing: 20
                // nodeType, nodeName, nodeSize lấy từ Roles trong fileNodeModel.cpp/h
                Text { text: nodeType === 1 ? "📁" : "📄"; font.pixelSize: 18; Layout.preferredWidth: 40 }
                Text { text: nodeName; color: "#f1f5f9"; Layout.preferredWidth: 260; elide: Text.ElideRight }
                Text { text: "Only you"; color: "#94a3b8"; Layout.preferredWidth: 150 }
                Text {
                    text: nodeType === 1 ? "--" : "File"
                    color: "#94a3b8"; Layout.fillWidth: true
                }
            }

            // Đường line ngăn cách các dòng
            Rectangle {
                anchors.bottom: parent.bottom
                width: parent.width
                height: 1
                color: "#1A2535"
            }
        }
    }
    // Khai báo MenuPopup và xử lý các tín hiệu
    MenuPopup {
        id: fileContextMenu

        onExportRequested: {
            // Nhận 'selectedId' và 'selectedName' từ MenuPopup và đẩy vào ExportPopup
            exportConfirmDialog.fileId = selectedId
            exportConfirmDialog.fileName = selectedName
            exportConfirmDialog.open()
        }

        onDeleteRequested: function(id, name) {
            console.log("Yêu cầu xóa mục: " + name + " (ID: " + id + ")")
        }
    }

    ExportPopup {
        id: exportConfirmDialog
    }
}
