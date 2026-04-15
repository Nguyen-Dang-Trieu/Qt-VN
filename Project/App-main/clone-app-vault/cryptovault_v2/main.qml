import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import "View"
import "View/LoginView"
import "View/SignupView"

ApplicationWindow {
    visible: true

    width: Screen.width * 0.8
    height: Screen.height * 0.8

    minimumWidth: 600
    minimumHeight: 450

    title: "File Vault"

    // Bộ điều hướng chính của ứng dụng
        StackView {
            id: stackView
            anchors.fill: parent
            // Khởi chạy ứng dụng sẽ vào thẳng trang Login
            initialItem: loginViewComp
        }

        // ==========================================
        // KHAI BÁO CÁC COMPONENTS (MÀN HÌNH)
        // ==========================================

        Component {
            id: loginViewComp
            LoginView {
                // Khi người dùng bấm "Sign up" -> Đẩy trang Username lên (Bước 1)
                onSignUpRequested: stackView.push(usernameViewComp)

                // Khi người dùng đăng nhập thành công -> Xóa trang Login và thế chỗ bằng App chính
                onLoginSuccess: stackView.replace(null, folderTreeViewComp)
            }
        }

        Component {
            id: usernameViewComp
            UsernameView {
                // Bấm "Back" -> Rút trang Username ra, trở về Login
                onBackToLoginRequested: stackView.pop()

                // Bấm "Next Step" -> Đẩy trang Password lên (Bước 2)
                onNextToCreatePasswordRequested: stackView.push(passwordViewComp)
            }
        }

        Component {
            id: passwordViewComp
            PasswordView {
                // Bấm "Back" -> Rút trang Password ra, trở về Username
                onBackToUsernameRequested: stackView.pop()

                // Bấm "Next Step" (Hoàn tất) -> Xóa toàn bộ lịch sử đăng ký/đăng nhập và đi thẳng vào App chính
                //onNextToConfirmationRequested: stackView.replace(null, folderTreeViewComp)
            }
        }

        Component {
            id: folderTreeViewComp
            FolderTreeView {
                // Nếu sau này bạn có nút Logout ở trong FolderTreeView,
                // bạn có thể gọi một signal và dùng lệnh sau để về lại trang đăng nhập:
                // onLogoutRequested: stackView.replace(null, loginViewComp)
            }
        }
}

