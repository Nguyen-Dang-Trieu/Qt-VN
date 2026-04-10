import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: mainContentRoot
    Layout.fillWidth: true
    Layout.fillHeight: true
    color: "#0f172a"

    property string activeVaultName: ""
    property string activeVaultPassword: ""

    // --- LOGIC ĐIỀU HƯỚNG ---
    // Khởi tạo stack với Root (ID = 1)
    property var pathStack: [{"n": "Root", "id": 1}]

    signal fileSelected(var fileData)

    // Hàm hỗ trợ để lấy thư mục hiện tại nhanh
    function getCurrentFolderId() {
        return pathStack[pathStack.length - 1].id
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 40
        spacing: 24

        // 1. TIÊU ĐỀ VAULT
        Text {
            text: mainContentRoot.activeVaultName !== "" ? "Vault: " + mainContentRoot.activeVaultName : "Select a Vault"
            font.pixelSize: 28; font.bold: true; color: "#f8fafc"
        }

        // 2. --- THANH ĐIỀU HƯỚNG (BREADCRUMBS) ---
        // Thêm một khung bao quanh thanh điều hướng để tách biệt với tiêu đề
        RowLayout {
            Layout.fillWidth: true
            spacing: 5

            Repeater {
                model: pathStack
                delegate: Row {
                    spacing: 5
                    Text {
                        text: modelData.n
                        // Nếu là mục cuối cùng (thư mục đang đứng) thì màu xanh sáng, ngược lại màu xám
                        color: index === pathStack.length - 1 ? "#38bdf8" : "#94a3b8"
                        font.bold: true
                        font.pixelSize: 14

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                if (index < pathStack.length - 1) {
                                    var targetId = modelData.id;
                                    // Gọi C++ load dữ liệu mới từ DB
                                    fileNodeModel.loadFolder(targetId);
                                    // Cắt bỏ các thư mục cấp con trong stack UI
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

        // 3. KHU VỰC HIỂN THỊ DANH SÁCH
        Rectangle {
            Layout.fillWidth: true; Layout.fillHeight: true
            color: "#1e293b"; radius: 12; clip: true

            ColumnLayout {
                anchors.fill: parent
                spacing: 0
                // --- THANH TIÊU ĐỀ CÁC CỘT ---
                Rectangle {
                    Layout.fillWidth: true; height: 50
                    color: "#334155"
                    RowLayout {
                        anchors.fill: parent; anchors.leftMargin: 20; spacing: 20
                        Text { text: "Type"; color: "#94a3b8"; font.bold: true; Layout.preferredWidth: 40 }
                        Text { text: "File Name"; color: "#94a3b8"; font.bold: true; Layout.preferredWidth: 260 }
                        Text { text: "Access"; color: "#94a3b8"; font.bold: true; Layout.preferredWidth: 150 }
                        Text { text: "Modified"; color: "#94a3b8"; font.bold: true; Layout.fillWidth: true }
                    }
                }

                // --- DANH SÁCH FILE/FOLDER (Dùng model từ C++) ---
                ListView {
                    id: fileListView
                    Layout.fillWidth: true; Layout.fillHeight: true
                    model: fileNodeModel
                    delegate: fileDelegate
                    clip: true

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
                onDropped: (drop) => {
                               if (drop.hasUrls) {
                                   if (activeVaultName === "" || activeVaultPassword === "") return;

                                   for (var i = 0; i < drop.urls.length; i++) {
                                       let rawPath = drop.urls[i].toString()
                                       // Gọi backend lưu vào DB dưới parentId hiện tại
                                       let success = vaultController.copyPathToVault(rawPath, activeVaultName, activeVaultPassword)
                                       if (success) {
                                           // Refresh lại model để hiển thị file vừa thêm
                                           fileNodeModel.loadFolder(getCurrentFolderId())
                                       }
                                   }
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
                onClicked: {
                    // Logic chọn file (Signal ra ngoài)
                    mainContentRoot.fileSelected({"name": nodeName, "id": nodeId})
                }
                onDoubleClicked: {
                    if (nodeType === 1) { // Nếu là Thư mục (Dir)
                        // 1. Cập nhật Breadcrumb UI
                        pathStack = [...pathStack, {"n": nodeName, "id": nodeId}]

                        // 2. Gọi C++ load dữ liệu của thư mục con này
                        fileNodeModel.loadFolder(nodeId)
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
}
