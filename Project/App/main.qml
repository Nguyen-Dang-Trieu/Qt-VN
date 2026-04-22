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

    // --- BIẾN TRUNG GIAN ĐỂ LƯU USERNAME TRONG QUÁ TRÌNH SIGNUP ---
        property string tempSignupUsername: ""

        StackView {
            id: stackView
            anchors.fill: parent
            initialItem: loginViewComp
        }

        // ==========================================
        // KHAI BÁO CÁC COMPONENTS (MÀN HÌNH)
        // ==========================================

        Component {
            id: loginViewComp
            LoginView {
                onSignUpRequested: stackView.push(usernameViewComp)
                onLoginSuccess: stackView.replace(null, folderTreeViewComp)
            }
        }

        Component {
            id: usernameViewComp
            UsernameView {
                onBackToLoginRequested: stackView.pop()

                onNextToCreatePasswordRequested: function(username) {
                    tempSignupUsername = username // Lưu tạm vào main.qml
                    stackView.push(passwordViewComp)
                }
            }
        }

        Component {
            id: passwordViewComp
            PasswordView {
                onBackToUsernameRequested: stackView.pop()

                onSignUpCompleted: stackView.replace(null, folderTreeViewComp)
            }
        }

        Component {
            id: folderTreeViewComp
            FolderTreeView {
               onLogoutRequested: stackView.replace(null, loginViewComp)
            }
        }
}

