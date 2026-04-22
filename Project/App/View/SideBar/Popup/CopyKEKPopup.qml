import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Popup {
    id: rootKekPopup
    width: 480
    // Không set cố định height để Popup tự động co giãn theo nội dung
    modal: true
    focus: true
    anchors.centerIn: Overlay.overlay

    // Thuộc tính NoAutoClose ép người dùng phải bấm nút Xác nhận ở dưới cùng mới tắt được
    closePolicy: Popup.NoAutoClose

    // Biến nhận giá trị KEK từ C++ hoặc JS truyền vào
    property string kekString: "KEK-DEFAULT-XXX"

    // Tín hiệu phát ra khi user đã xác nhận lưu xong
    signal copyConfirmed()

    background: Rectangle {
        color: "#1e293b"
        radius: 12
        border.color: "#334155"
        border.width: 1
    }

    contentItem: ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 20

        // --- 1. HEADER & CẢNH BÁO ---
        ColumnLayout {
            spacing: 8
            Layout.fillWidth: true

            Text {
                text: "⚠️ Vault Recovery Key"
                font.pixelSize: 20
                font.bold: true
                color: "#fca5a5" // Chữ màu đỏ cảnh báo
            }

            Text {
                text: "This is your Master Recovery Key. If you forget your password, this key is the ONLY way to recover your encrypted files. Please copy and store it in a safe place immediately."
                font.pixelSize: 13
                color: "#94a3b8"
                wrapMode: Text.Wrap
                Layout.fillWidth: true
                lineHeight: 1.4
            }
        }

        // --- 2. KHU VỰC HIỂN THỊ VÀ COPY KEK ---
        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            TextField {
                id: kekInput
                text: rootKekPopup.kekString
                readOnly: true // Khóa không cho user gõ phím sửa chữ
                selectByMouse: true

                Layout.fillWidth: true
                Layout.preferredHeight: 44
                color: "#34d399" // Chữ KEK màu xanh ngọc nổi bật
                font.pixelSize: 14
                font.bold: true
                font.letterSpacing: 1
                horizontalAlignment: TextInput.AlignHCenter

                background: Rectangle {
                    color: "#0f172a"
                    radius: 8
                    border.color: "#334155"
                }
            }

            // Nút Copy
            Button {
                id: copyBtn
                Layout.preferredWidth: 110
                Layout.preferredHeight: 44

                property bool isCopied: false

                contentItem: Text {
                    text: copyBtn.isCopied ? "Copied! ✓" : "Copy"
                    color: copyBtn.isCopied ? "#000000" : "white"
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle {
                    // Đổi màu nền sang Xanh khi copy thành công
                    color: copyBtn.isCopied ? "#34d399" : (copyBtn.pressed ? "#0369a1" : "#0284c7")
                    radius: 8
                }

                onClicked: {
                    // Dùng hàm có sẵn của TextField để bôi đen và copy vào Clipboard
                    kekInput.selectAll()
                    kekInput.copy()
                    kekInput.deselect()

                    isCopied = true
                    resetCopyText.start() // Kích hoạt bộ đếm thời gian
                }

                // Hẹn giờ 2 giây sau đó đổi chữ "Copied! ✓" về lại "Copy KEK"
                Timer {
                    id: resetCopyText
                    interval: 2000
                    onTriggered: copyBtn.isCopied = false
                }
            }
        }

        Item { Layout.preferredHeight: 10 } // Spacer tạo khoảng trống

        // --- 3. NÚT XÁC NHẬN ---
        Button {
            Layout.fillWidth: true
            Layout.preferredHeight: 44
            Layout.bottomMargin: 20
            contentItem: Text {
                text: "I have saved it securely"
                color: "white"
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: parent.pressed ? "#334155" : "transparent"
                border.color: "#475569"
                border.width: 1
                radius: 8
            }

            onClicked: {
                rootKekPopup.copyConfirmed()
                rootKekPopup.close()
            }
        }
    }
}
