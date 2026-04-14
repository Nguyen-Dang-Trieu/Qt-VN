// File: ExportPopup.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.1

Popup { // ----------------------------------------- Popup cho SAVE AS
    id: exportConfirmDialog
    anchors.centerIn: parent
    width: 480
    modal: true
    focus: true
    padding: 0

    property int    fileId: -1
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
                        console.log("[VIEW] path save as: " + exportPathField.text + " (../MainContent.qml)");
                        console.log("[VIEW] file ID: " + exportConfirmDialog.fileId + " (../MainContent.qml)");
                        console.log("[VIEW] file Name: " + exportConfirmDialog.fileName + " (../MainContent.qml)");
                        console.log("[VIEW] name Vault: " + mainContentRoot.activeVaultName + " (../MainContent.qml)");
                        console.log("[VIEW] pass Vault: " + mainContentRoot.activeVaultPassword + " (../MainContent.qml)");
                        vaultController.exportFileFromVault(
                            exportPathField.text,                // text là đường dẫn đến thư mục mà user cần SAVE AS
                            mainContentRoot.activeVaultName,     // Tên của vault name mà user
                            mainContentRoot.activeVaultPassword, // Pass cua vault
                            exportConfirmDialog.fileId,          // ID của file được clicked
                            exportConfirmDialog.fileName         // Name của file được click

                        )
                        exportConfirmDialog.close()   // ĐÓNG POPUP sau khi xử lí xong
                    }
                }
            }
        }
    }

    FolderDialog {
        id: folderDialog
        title: "Select Destination Folder"
        onAccepted: {
            let path = folder.toString().replace(/^(file:\/{3})/, "") // Xử lý path Windows/Linux
            exportPathField.text = path
        }
    }
}
