import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import "View"

Window {
    width: 1280
    height: 720
    minimumWidth: 600
    minimumHeight: 450

    visible: true
    title: "File Vault"

//    StackView {
//        id: stackView
//        anchors.fill: parent
//        initialItem: _loginView_
//    }

//    Component {
//        id: _loginView_
//        LoginView {
//            onLoginSuccess: stackView.replace(_vaultView_)
//            onGoToRegister: stackView.push(_registerView_)
//        }
//    }

////    LoginView {
////            anchors.fill: parent
////    }


//    Component {
//        id: _registerView_
//        RegisterView {
//            onBackToLogin: stackView.pop()
//        }
//    }

//    Component {
//        id: _vaultView_
//        VaultView {
//            onLogout: stackView.replace(loginView)
//        }
//    }

    // Test view component
    FolderTreeView {
            anchors.fill: parent
    }

}

