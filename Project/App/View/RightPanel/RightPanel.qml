import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: rightPanelRoot
    width: 320
    Layout.fillHeight: true
    color: "#060913"

    Rectangle {
        width: 1
        height: parent.height
        color: "#1A2535"
        anchors.left: parent.left
    }

    property var selectedFileData: null

    // --- HÀM FORMAT DỮ LIỆU ---
    // Chuyển mili-giây thành định dạng "Oct 12, 2025"
    function formatDate(sec) {
        if (!sec) return "--"
        // CẬP NHẬT: Nhân 1000 để đổi từ Giây (C++) sang Mili-giây (JavaScript)
        var date = new Date(Number(sec) * 1000)
        var months = ["Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"]
        return months[date.getMonth()] + " " + ("0" + date.getDate()).slice(-2) + ", " + date.getFullYear()
    }

    // Chuyển đổi Bytes sang KB, MB, GB...
    function formatBytes(bytes) {
        if (bytes === undefined || bytes === null) return "--"
        if (bytes === 0) return "0 B"
        const k = 1024
        const sizes = ['B', 'KB', 'MB', 'GB', 'TB']
        const i = Math.floor(Math.log(bytes) / Math.log(k))
        return parseFloat((bytes / Math.pow(k, i)).toFixed(2)) + ' ' + sizes[i]
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 24

        // --- 1. TITLE SECTION ---
        ColumnLayout {
            spacing: 8
            Text {
                text: selectedFileData ? selectedFileData.fileName : "Select a file or folder"
                font.pixelSize: 20
                font.bold: true
                color: "white"
                elide: Text.ElideRight
                Layout.fillWidth: true
            }
            Text {
                // Giả lập hiển thị định dạng mở rộng (VD: PPTX, DOCX) hoặc FOLDER
                text: selectedFileData ? (selectedFileData.fileType === 1 ? "FOLDER" : "FILE") : "---"
                color: "#8E9AAB"
                font.pixelSize: 11
                font.bold: true
                font.letterSpacing: 1.5
            }
        }

        // --- 2. PREVIEW BOX ---
        Rectangle {
            Layout.fillWidth: true
            height: 180
            color: "#0D1321"
            radius: 8
            border.color: "#1A2535"
            border.width: 1

            Text {
                anchors.centerIn: parent
                // Nếu là Folder hiện Icon Folder, nếu File hiện Icon File
                text: selectedFileData ? (selectedFileData.fileType === 1 ? "📁" : "📄") : ""
                font.pixelSize: 48
                opacity: selectedFileData ? 1.0 : 0.3
            }
        }

        // --- 3. INFO LIST ---
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 16

            Repeater {
                // Map các trường data nhận được từ C++ vào UI
                // DB có atime (Access) và mtime (Modified). UI đang cần "Created", ta có thể tạm dùng atime làm Created.
                model: {
                    if (!selectedFileData) return []

                    // Các thông tin cơ bản luôn hiển thị
                    let infoList = [
                            //{ k: "Last Access", v: formatDate(selectedFileData.atime) },
                            { k: "Modified", v: formatDate(selectedFileData.mtime) }
                        ]

                    if (selectedFileData.fileType !== 1) {
                        infoList.push({ k: "Size", v: formatBytes(selectedFileData.size) })
                    }
                    return infoList
                }

                delegate: RowLayout {
                    Layout.fillWidth: true

                    Text {
                        text: modelData.k
                        color: "#5E6A7A"
                        font.pixelSize: 13
                    }

                    Item { Layout.fillWidth: true }

                    Text {
                        text: modelData.v
                        color: "white"
                        font.pixelSize: 13
                        font.bold: true
                    }
                }
            }
        }

        Item { Layout.fillHeight: true }
    }
}
