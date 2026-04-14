import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: rightPanelRoot
    width: 320
    Layout.fillHeight: true
    color: "#060913" // Đen sâu đồng bộ với nền ứng dụng

    // Viền mỏng bên trái để ngăn cách tinh tế với MainContent
    Rectangle {
        width: 1
        height: parent.height
        color: "#1A2535"
        anchors.left: parent.left
    }

    // Property hứng dữ liệu
    property var selectedFileData: null

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 24

        // --- 1. TITLE SECTION ---
        ColumnLayout {
            spacing: 8
            Text {
                text: selectedFileData ? selectedFileData.fileName : "Select a file"
                font.pixelSize: 20
                font.bold: true
                color: "white"
                elide: Text.ElideRight // Cắt chữ bằng dấu ... nếu tên file quá dài
                Layout.fillWidth: true
            }
            Text {
                text: selectedFileData ? selectedFileData.fileType.toUpperCase() : "---"
                color: "#4EA8FE" // Dùng màu Neon Blue làm điểm nhấn cho loại file
                font.pixelSize: 11
                font.bold: true
                font.letterSpacing: 1.5 // Giãn chữ cho ngầu
            }
        }

        // --- 2. PREVIEW BOX ---
        Rectangle {
            Layout.fillWidth: true
            height: 180
            color: "#0D1321" // Nền hộp tối giống các form nhập liệu
            radius: 8
            border.color: "#1A2535" // Viền hộp bo góc đồng bộ
            border.width: 1

            Text {
                anchors.centerIn: parent
                text: selectedFileData ? (selectedFileData.fileType === "zip" ? "📦" : "📄") : "🔍"
                font.pixelSize: 48
                opacity: selectedFileData ? 1.0 : 0.3 // Làm mờ icon nếu chưa chọn file
            }
        }

        // --- 3. INFO LIST ---
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 16

            Repeater {
                model: selectedFileData ? [
                    { k: "Created", v: "Sep 2020" },
                    { k: "Modified", v: selectedFileData.modified || "--" },
                    { k: "Size", v: "302 MB" }
                ] : []

                delegate: RowLayout {
                    Layout.fillWidth: true

                    Text {
                        text: modelData.k.toUpperCase()
                        color: "#8E9AAB" // Chữ tiêu đề phụ màu xám xanh
                        font.pixelSize: 11
                        font.bold: true
                        font.letterSpacing: 1
                    }

                    Item { Layout.fillWidth: true } // Đẩy giá trị sang sát lề phải

                    Text {
                        text: modelData.v
                        color: "white"
                        font.pixelSize: 13
                    }
                }
            }
        }

        Item { Layout.fillHeight: true } // Ép nút Download xuống dưới cùng

        // --- 4. ACTION BUTTON ---
        Button {
            Layout.fillWidth: true
            Layout.preferredHeight: 48
            visible: selectedFileData !== null // Chỉ hiện nút khi có file được chọn

            contentItem: RowLayout {
                anchors.centerIn: parent
                spacing: 8
                Text {
                    text: "DOWNLOAD ARCHIVE"
                    color: "#000000" // Chữ đen đặc trưng trên nền màu sáng
                    font.bold: true
                    font.pixelSize: 13
                    font.letterSpacing: 1
                }
                Text {
                    text: "↓"
                    color: "#000000"
                    font.bold: true
                    font.pixelSize: 16
                }
            }

            background: Rectangle {
                // Hiệu ứng đổi màu khi click chuột (giống trang Sign Up)
                color: parent.down ? "#3A80C9" : "#4EA8FE"
                radius: 6
            }

            onClicked: {
                console.log("Download clicked for:", selectedFileData.fileName)
            }
        }
    }
}
