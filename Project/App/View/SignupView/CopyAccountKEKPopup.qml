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

    // Thuộc tính NoAutoClose ép người dùng phải bấm nút Xác nhận ở dưới cùng mới tắt được [cite: 842]
    closePolicy: Popup.NoAutoClose

    // Biến nhận giá trị KEK từ C++ hoặc JS truyền vào [cite: 842]
    property string kekString: "KEK-DEFAULT-XXX"

    // Tín hiệu phát ra khi user đã xác nhận lưu xong [cite: 843]
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
                text: "⚠️ Account Recovery Key (KEK)"
                font.pixelSize: 20
                font.bold: true
                color: "#fca5a5" // Chữ màu đỏ cảnh báo [cite: 844, 845]
            }

            Text {
                text: "This is your Account Recovery Key. If you forget your login password, this key is the ONLY way to recover your account and access your Vaults. Please copy and store it in a safe place immediately."
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
                readOnly: true // Khóa không cho user gõ phím sửa chữ [cite: 849]
                selectByMouse: true

                Layout.fillWidth: true
                Layout.preferredHeight: 44
                color: "#34d399" // Chữ KEK màu xanh ngọc nổi bật [cite: 850]
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
                    text: copyBtn.isCopied ? "Copied! ✓" : "Copy KEK"
                    color: copyBtn.isCopied ? "#000000" : "white"
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle {
                    // Đổi màu nền sang Xanh khi copy thành công [cite: 856]
                    color: copyBtn.isCopied ? "#34d399" : (copyBtn.pressed ? "#0369a1" : "#0284c7")
                    radius: 8
                }

                onClicked: {
                    // Dùng hàm có sẵn của TextField để bôi đen và copy vào Clipboard [cite: 857]
                    kekInput.selectAll()
                    kekInput.copy()
                    kekInput.deselect()

                    isCopied = true
                    resetCopyText.start() // Kích hoạt bộ đếm thời gian [cite: 858]
                }

                // Hẹn giờ 2 giây sau đó đổi chữ "Copied! ✓" về lại "Copy KEK" [cite: 859]
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
