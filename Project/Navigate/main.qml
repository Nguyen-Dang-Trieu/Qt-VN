import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    visible: true; width: 600; height: 400
    title: "CryptVault - Cấu trúc cây DB"
    color: "#0f172a"

    /*
    Một array chứa một đối tượng duy nhất:
    - Key: "n" -> Value: "Root"
    - Key: "id" -> Value: 1

    */
    property var pathStack: [{"n": "Root", "id": 1}]


    ColumnLayout {
        anchors.fill: parent; anchors.margins: 20; spacing: 15

        // --- BREADCRUMBS ---
        Rectangle {
            width: childrenRect.width + 10 // Tự động co giãn theo nội dung
            height: childrenRect.height + 10
            color: "transparent"          // Giữ nền trong suốt
            border.color: "gray"           // Màu viền để debug
            border.width: 1
            radius: 4                     // Bo góc cho đẹp nếu muốn giữ lại luôn

            RowLayout {
                anchors.centerIn: parent
                spacing: 5
                Repeater {
                    model: pathStack
                    delegate: Row {
                        Text {
                            text: modelData.n // Lấy tên thư mục để hiển thị

                            // index (builin) - vị trí của mục hiện tại trong danh sách
                            color: index === pathStack.length - 1 ? "#38bdf8" : "#94a3b8" // pathStack.length - 1 - vị trí cuối cùng
                            font.bold: true // Text đậm

                            // Vùng để ta nhấn vào Text đó
                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    // 1. Lưu lại ID mục tiêu vào biến local (rất quan trọng)
                                    var targetId = modelData.id;

                                    // 2. Gọi logic C++ TRƯỚC khi thay đổi UI
                                    // Việc này đảm bảo engine vẫn tìm thấy fileNodeModel trong context ổn định
                                    fileNodeModel.loadFolder(targetId);

                                    // 3. Sau đó mới cập nhật thanh điều hướng (Breadcrumb)
                                    if (index < pathStack.length - 1) {
                                        var newStack = pathStack.slice(0, index + 1);
                                        pathStack = newStack;
                                    }
                                }
                            }
                        }
                        Text {
                            text: " > "
                            color: "#475569"
                            visible: index < pathStack.length - 1
                        }
                    }
                }
            }
        }

        // --- DANH SÁCH FILE/FOLDER ---
        Rectangle {
            Layout.fillWidth: true; Layout.fillHeight: true
            color: "#1e293b"; radius: 8; clip: true
            border.color: "red"


            // Cac bien nodeType, nodeName, nodeSize duoc dinh nghia trong filenodemodel.h
            ListView {
                id: listView
                anchors.fill: parent
                model: fileNodeModel // Lay du lieu tu DB -> gọi rowCount() để  biết có bao nhiêu delegate
                delegate: ItemDelegate {
                    width: listView.width

                    // NƠI DATA ĐƯỢC LẤY TỪ C++, GỌI HÀM data()
                    contentItem: RowLayout {
                        spacing: 15
                        Text { text: nodeType === 1 ? "📁" : "📄"; font.pixelSize: 18 }
                        Text { text: nodeName; color: "green"; Layout.fillWidth: true }
                        Text { text: nodeType === 2 ? (nodeSize + " B") : ""; color: "yellow" }
                    }


                    background: Rectangle {
                        color: hovered ? "#334155" : "transparent" // transparent: trong suot
                        border.color: "white"
                    }

                    // ---------------------------- Thêm item vào trong pathStack
                    onDoubleClicked: {
                        if (nodeType === 1) { // Kiểm tra nếu mục được nhấn là Folder

                            // THÊM DATA VÀO TRONG PATHSTACK
                            // Sử dụng toán tử spread [...] hoặc concat để tạo mảng mới
                            pathStack = [...pathStack, {"n": nodeName, "id": nodeId}]

                            // Call backend C++ để lấy dữ liệu lên UI
                            fileNodeModel.loadFolder(nodeId)
                        }
                    }
                }
            }
        }
    }
}
