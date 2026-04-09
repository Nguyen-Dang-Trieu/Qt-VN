import QtQuick 2.15
import QtQuick.Window 2.15

Window {
    width: 600
    height: 400
    visible: true
    title: "Vault DropArea Demo"

    Rectangle {
        id: dropZoneRect
        anchors.centerIn: parent
        width: 400
        height: 300

        // Hiệu ứng thị giác: Đổi màu khi có vật thể kéo lơ lửng trên nó
        color: dropArea.containsDrag ? "#e3f2fd" : "#f5f5f5"
        border.color: dropArea.containsDrag ? "#2196f3" : "#bdbdbd"
        border.width: 3
        radius: 12

        Text {
            anchors.centerIn: parent
            text: dropArea.containsDrag ? "Thả vào đây!" : "Kéo File/Folder vào vùng này"
            font.pixelSize: 18
            color: dropArea.containsDrag ? "#1976d2" : "#757575"
        }

        DropArea {
            id: dropArea
            anchors.fill: parent

            // Xử lý khi người dùng nhả chuột
            onDropped: (drop) => {
                if (drop.hasUrls) {
                    for (var i = 0; i < drop.urls.length; i++) {
                        // Lấy đường dẫn gốc
                        let rawUrl = drop.urls[i].toString();
                        console.log("Đã thả mục:", rawUrl);

                        // Gọi C++ copy path vào Vault
                        let success = vaultCtrl.copyPathToVault(rawUrl, "MySecretVault");

                        if(success) {
                            console.log("Đã đưa vào Vault thành công!");
                        } else {
                            console.log("Có lỗi xảy ra khi copy!");
                        }
                    }
                }
            }
        }
    }
}