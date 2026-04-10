import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: rightPanelRoot
    width: 320; Layout.fillHeight: true
    color: "#0f172a"; border.color: "#1e293b"

    // Property hứng dữ liệu
    property var selectedFileData: null

    ColumnLayout {
        anchors.fill: parent; anchors.margins: 24; spacing: 24

        // Title Section
        ColumnLayout {
            spacing: 8
            Text {
                text: selectedFileData ? selectedFileData.fileName : "Select a file"
                font.pixelSize: 20; font.bold: true; color: "#f8fafc"
            }
            Text {
                text: selectedFileData ? selectedFileData.fileType.toUpperCase() : "---"
                color: "#64748b"; font.pixelSize: 12
            }
        }

        // Preview Box
        Rectangle {
            Layout.fillWidth: true; height: 180; color: "#1e293b"; radius: 12
            Text {
                anchors.centerIn: parent
                text: selectedFileData ? (selectedFileData.fileType === "zip" ? "📦" : "📄") : "🔍"
                font.pixelSize: 48
            }
        }

        // Info List
        ColumnLayout {
            Layout.fillWidth: true; spacing: 16
            Repeater {
                model: selectedFileData ? [
                    { k: "Created", v: "Sep 2020" },
                    { k: "Modified", v: selectedFileData.modified },
                    { k: "Size", v: "302 MB" }
                ] : []
                delegate: RowLayout {
                    Layout.fillWidth: true
                    Text { text: modelData.k; color: "#64748b" }
                    Item { Layout.fillWidth: true }
                    Text { text: modelData.v; color: "#f1f5f9"; font.bold: true }
                }
            }
        }

        Item { Layout.fillHeight: true }

        Button {
            Layout.fillWidth: true; height: 48
            visible: selectedFileData !== null
            background: Rectangle { color: "#6366f1"; radius: 8 }
            contentItem: Text { text: "Download Archive"; color: "white"; font.bold: true; horizontalAlignment: Text.AlignHCenter }
        }
    }
}
