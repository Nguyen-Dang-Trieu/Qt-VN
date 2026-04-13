# Property trong QML

property <kiểu dữ liệu> <tên thuộc tính> : <giá trị mặc định>

## Kiểu dữ liệu cơ bản
int, double, bool, string, color

- Tạo bí danh (alias) -> property alias

property change signal: mỗi property đều có một signal thay đổi tự động sinh ra: <property name>Changed

Trong QML, khi khai báo một property, hệ thống sẽ tự động sinh ra 1 signal có tên dạng <property name>Changed
- Signal này phát ra (emit) mỗi khi giá trị của property thay đổi
- Ta có thể đón nhận signal này bằng cách viết 1 handler vs cú pháp on<property name>Changed


VD:
~~~
Sibar.qml
property int selectedVaultIndex: -1             // Luu tru index cua vault dc "Click"
~~~

Khi user click vao Vault muon theo tac, thi se phat ra mot signal. Luc nay, onSelectedVaultIndexChanged() se duoc QML call de xu li
~~~
// FolderTreeView.qml
onSelectedVaultIndexChanged: {
    if (selectedVaultIndex !== -1) {
        var data = sideBarArea.vaultModel.get(selectedVaultIndex)
        if (data) {
            // 1. Truyền tên Vault
            mainArea.activeVaultName     = data.name
            mainArea.activeVaultPath     = data.path
            mainArea.activeVaultPassword = data.password
            mainArea.clearFiles()

            // console.log("[VIEW] FolderView: Đã đồng bộ Vault " + data.name + " và Password sang MainContent")
        }
    } else {
        // Reset khi không chọn vault nào
        mainArea.activeVaultName = ""
        mainArea.activeVaultPassword = ""
    }
}
~~~