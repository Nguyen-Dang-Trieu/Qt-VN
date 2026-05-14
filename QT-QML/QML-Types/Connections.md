Để hiểu rõ về Connections, hãy tưởng tượng nó giống như một "Trạm nghe lén điện thoại".

Thông thường, trong QML, chỉ có thể xử lý tín hiệu (signal) của một thẻ nếu viết code trực tiếp bên trong thẻ đó. Nhưng Connections cho phép bạn đứng từ xa để lắng nghe và hành động.

Ví dụ xử lí trực tiếp
~~~
MouseArea {
    anchors.fill: parent
    
    // ĐÂY LÀ XỬ LÝ TRỰC TIẾP
    onClicked: {
        console.log("Bạn vừa bấm vào tôi!")
    }
}
~~~

Ví dụ: Có thuộc tính thay đổi
~~~
Item {
    property int count: 0
    
    // ĐÂY LÀ XỬ LÝ TRỰC TIẾP (khi biến count thay đổi)
    onCountChanged: {
        console.log("Số đếm mới là: " + count)
    }
}
~~~

## Dưới đây là 3 lý do chính khiến ta cần dùng nó:

### 1. Lắng nghe từ xa (Cái này quan trọng nhất)
Giả sử bạn có một cái Store (như MainStore.chatBotStore trong dự án của bạn). Store này nằm ở một file C++ hoặc một file QML khác. Bạn không thể chui vào file Store đó để viết code thay đổi giao diện được.

Thay vào đó, tại giao diện, bạn dùng Connections để "hóng" tín hiệu từ Store đó:

~~~
Item {
    // Tôi đứng ở giao diện, nhưng tôi muốn hóng chuyện từ ChatBotStore
    Connections {
        target: MainStore.chatBotStore
        
        // Khi Store phát tín hiệu "isStreamingChanged" (đang gửi tin nhắn)
        function onIsStreamingChanged() {
            if (MainStore.chatBotStore.isStreaming) {
                console.log("Bot đang gõ chữ kìa, hiện hiệu ứng loading thôi!")
            }
        }
    }
}
~~~

### 2. Khi đối tượng mục tiêu chưa tồn tại (Dynamic Objects)
Hãy nghĩ về thẻ Loader. Khi ứng dụng mới chạy, Loader chưa tải nội dung lên, nên bạn không thể viết code xử lý cho nội dung đó được.

Connections giúp bạn thiết lập sẵn một "đường dây chờ". Khi nào Loader tải xong nội dung, nó sẽ tự động kết nối.

~~~qml
Loader { id: myLoader; source: "ChatWindow.qml" }

Connections {
    target: myLoader.item // "item" là nội dung thực sự khi đã load xong
    ignoreUnknownSignals: true // Nếu chưa load xong thì cứ lờ đi, đừng báo lỗi
    
    function onMessageSent() { 
        console.log("Người dùng vừa gửi tin từ cửa sổ chat mới load!") 
    }
}
~~~