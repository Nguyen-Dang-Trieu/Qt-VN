import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.1
import QtGraphicalEffects 1.15
import "Popup"

Rectangle {
    id: mainContentRoot
    Layout.fillWidth: true
    Layout.fillHeight: true
    color: "#060913"

    property string activeVaultName: ""
    property string activeVaultPath: ""
    property string activeVaultPassword: ""

    // --- LOGIC ĐIỀU HƯỚNG ---
    // Khởi tạo stack với Root (ID = 1)
    property var pathStack: [{"name": "Root", "id": 1}] // Khởi tạo stack với Root (ID = 1)

    signal fileSelected(var fileData)
    signal logoutRequested()

    // Hàm hỗ trợ để lấy thư mục hiện tại nhanh
    function getCurrentFolderId() {
        return pathStack[pathStack.length - 1].id
    }

    function formatDate(sec) {
        if (!sec) return "--"
        // Lưu ý: C++ lưu dạng Giây (Seconds), Javascript Date cần Mili-giây nên phải x1000
        var date = new Date(Number(sec) * 1000)
        var months = ["Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"]
        return months[date.getMonth()] + " " + ("0" + date.getDate()).slice(-2) + ", " + date.getFullYear()
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 15
        spacing: 24

        // 1. KHU VỰC HEADER

        // 2. --- THANH ĐIỀU HƯỚNG (BREADCRUMBS) ---
        // Thêm một khung bao quanh thanh điều hướng để tách biệt với tiêu đề
        RowLayout {
            Layout.fillWidth: true
            spacing: 5
            visible: mainContentRoot.activeVaultName !== ""
            Repeater {
                model: pathStack
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
                            cursorShape: Qt.PointingHandCursor
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

        // 3. KHU VỰC HIỂN THỊ DANH SÁCH
        Rectangle {
            Layout.fillWidth: true;
            Layout.fillHeight: true
            color: "#0D1321"
            radius: 8
            clip: true
            border.color: "#1A2535"
            border.width: 1
            visible: mainContentRoot.activeVaultName !== ""
            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                // --- THANH TIÊU ĐỀ CÁC CỘT ---
                Rectangle {
                    Layout.fillWidth: true; height: 50
                    color: "#121A2F"

                    // viền dưới ngăn cách header và list
                    Rectangle {
                        width: parent.width
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
                        //Text { text: "ACCESS"; color: "#8E9AAB"; font.bold: true; font.pixelSize: 11; font.letterSpacing: 1; Layout.preferredWidth: 150 }
                        Text { text: "MODIFIED"; color: "#8E9AAB"; font.bold: true; font.pixelSize: 11; font.letterSpacing: 1; Layout.fillWidth: true }
                    }
                }

                // --- DANH SÁCH FILE/FOLDER (Dùng model từ C++) ---
                ListView {
                    id: fileListView
                    Layout.fillWidth: true; Layout.fillHeight: true
                    model: listNodes
                    delegate: fileDelegate
                    clip: true

                    Text {
                        anchors.centerIn: parent
                        text: "No items in this folder"
                        color: "#5E6A7A"
                        font.pixelSize: 13
                        visible: fileListView.count === 0
                    }
                }
            }

            // NEW 2026-04-20
            Connections {
                target: vaultController // Tên biến C++ bạn đã đăng ký với QML

                function onImportFinished(success) {
                    if (success) {
                        console.log("Mã hóa hoàn tất! Đang làm mới danh sách...");
                        // Gọi hàm load dữ liệu của Model
                        listNodes.loadFolder(getCurrentFolderId());
                    } else {
                        console.log("Quá trình mã hóa có lỗi xảy ra.");
                    }
                }
            }

            // DropArea để kéo thả file vào thư mục hiện tại
            DropArea {
                anchors.fill: parent
                onDropped: (drop) => {
                               if (drop.hasUrls) {
                                   if (activeVaultName === "" || activeVaultPassword === "") return;
                                   let pathList = [];

                                   //                                   for (let i = 0; i < drop.urls.length; i++) {
                                   //                                       let rawPath = drop.urls[i].toString()
                                   //                                       // Gọi backend lưu vào DB dưới parentId hiện tại
                                   //                                       let success = vaultController.importDataToVaultAsync(rawPath, activeVaultName, activeVaultPassword)
                                   //                                       if (success) {
                                   //                                           // Refresh lại model để hiển thị file vừa thêm
                                   //                                           listNodes.loadFolder(getCurrentFolderId())
                                   //                                       }
                                   //                                   }
                                   for (let i = 0; i < drop.urls.length; i++) {
                                       pathList.push(drop.urls[i].toString());
                                   }

                                   // Gửi một lần duy nhất
                                   vaultController.importDataToVaultAsync(pathList, activeVaultName, activeVaultPassword);
                               }
                           }
            }
        }
    }

    // --- DELEGATE HIỂN THỊ TỪNG DÒNG ---
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
                    mainContentRoot.fileSelected({
                                                     "id": nodeId,
                                                     "fileName": nodeName,
                                                     "fileType": nodeType,
                                                     "atime": nodeAtime,   // Gọi biến Role từ model C++
                                                     "mtime": nodeMtime,   // Gọi biến Role từ model C++
                                                     "size": nodeLength    // Gọi biến Role từ model C++
                                                 })

                    // Xử lý riêng khi click CHUỘT PHẢI
                    if (mouse.button === Qt.RightButton) {
                        console.log("[DEBUG] Đã nhận click chuột phải, mở Menu cho: " + nodeName)

                        // Gán thông tin file vào Popup
                        fileContextMenu.targetNodeId = nodeId
                        fileContextMenu.targetNodeName = nodeName
                        fileContextMenu.targetNodeType = nodeType

                        // Chuyển đổi tọa độ chuột từ MouseArea ra tọa độ của mainContentRoot
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
                //Text { text: "Only you"; color: "#94a3b8"; Layout.preferredWidth: 150 }
                Text {
                    text: mainContentRoot.formatDate(nodeMtime)
                    color: "#94a3b8"
                    Layout.fillWidth: true
                }
//                Text {
//                    text: nodeType === 1 ? "Folder" : "File"
//                    color: "#94a3b8"; Layout.fillWidth: true
//                }
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
            exportConfirmDialog.fileId   = selectedId
            exportConfirmDialog.fileName = selectedName
            exportConfirmDialog.fileType = selectedType
            exportConfirmDialog.open()
        }

        onDeleteRequested: function(id, name) {
            //            console.log("[VIEW] Request to delete (file/folder): " + name + " (ID: " + id + ")" + " (../MainContent.qml)")
            //            console.log("[VIEW] password: " + mainContentRoot.activeVaultPassword + " (../MainContent.qml)")
            let del = vaultController.deleteFile(mainContentRoot.activeVaultName, mainContentRoot.activeVaultPassword, name, id)
            if(del === true) {
                listNodes.loadFolder(getCurrentFolderId())
            }
        }
    }

    ExportPopup {
        id: exportConfirmDialog
    }

    // 4. MÀN HÌNH CHỜ KHI KHÔNG CÓ VAULT NÀO ĐƯỢC CHỌN (BỔ SUNG MỚI HOÀN TOÀN)
    ColumnLayout {
        anchors.centerIn: parent
        visible: mainContentRoot.activeVaultName === "" // Chỉ hiện khi rỗng
        spacing: 16

        Text {
            text: "🔒"
            font.pixelSize: 54
            Layout.alignment: Qt.AlignHCenter
            color: "#1A2535"
        }

        Text {
            text: "No Vault Selected"
            color: "#5E6A7A"
            font.pixelSize: 22
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        Text {
            text: "Select or unlock a vault from your vault folder to view its contents."
            color: "#334155"
            font.pixelSize: 14
            Layout.alignment: Qt.AlignHCenter
        }
    }

}
