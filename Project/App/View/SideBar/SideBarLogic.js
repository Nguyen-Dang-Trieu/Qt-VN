// File: SideBarLogic.js

// -----------------------  RENAME VAULT -----------------------
function openDynamicRenamePopup(sidebarRoot, vaultController) {
    if (sidebarRoot.selectedVaultIndex === -1) return

    // Lấy model thông qua sidebarRoot
    let currentName = sidebarRoot.vaultModel.get(sidebarRoot.selectedVaultIndex).name

    let comp = Qt.createComponent("qrc:/View/SideBar/Popup/RenamePopup.qml")
    console.log("DEBUG: Trạng thái Component Rename:", comp.status);

    if (comp.status === Component.Ready){
        let popup = comp.createObject(sidebarRoot, {
                                          "currentName": currentName,
                                          "primaryBlue": sidebarRoot.primaryBlue,
                                          "hoverBlue": sidebarRoot.hoverBlue
                                      })

        popup.renameConfirmed.connect(function(newName) {
            let oldPath   = sidebarRoot.vaultModel.get(sidebarRoot.selectedVaultIndex).path
            let passVault

            console.log("[VIEW] old Path: " + oldPath + " (../SideBarLogic.js)")
            console.log("[VIEW] new Name: " + newName + " (../SideBarLogic.js)")

            // Gọi C++ logic
            let newPath = vaultController.renameVault(oldPath, newName, passVault)

            if (newPath !== "") {
                sidebarRoot.vaultModel.setProperty(sidebarRoot.selectedVaultIndex, "name", newName)
                sidebarRoot.vaultModel.setProperty(sidebarRoot.selectedVaultIndex, "path", newPath)
                popup.close()
            } else {
                console.log("Lỗi: Không thể đổi tên Vault")
            }
        })

        popup.closed.connect(function() { popup.destroy() })
        popup.open()
    } else {
        console.error("Lỗi tải RenamePopup:", comp.errorString())
    }
}

// -----------------------  DELETE VAULT -----------------------
function openDynamicDeletePopup(sidebarRoot, vaultController) {
    if (sidebarRoot.selectedVaultIndex === -1) return

    let vName = sidebarRoot.vaultModel.get(sidebarRoot.selectedVaultIndex).name
    let comp = Qt.createComponent("qrc:/View/SideBar/Popup/DeletePopup.qml")

    console.log("DEBUG: Trạng thái Delete Popup:", comp.status);

    if (comp.status === Component.Ready) {
        let popup = comp.createObject(sidebarRoot, { "vaultName": vName })

        popup.deleteConfirmed.connect(function() {
            let targetPath = sidebarRoot.vaultModel.get(sidebarRoot.selectedVaultIndex).path
            if (vaultController.deleteVault(targetPath)) {
                sidebarRoot.vaultModel.remove(sidebarRoot.selectedVaultIndex)
                sidebarRoot.selectedVaultIndex = -1
                popup.close()
            } else {
                console.log("Lỗi: Không thể xóa Vault")
            }
        })

        popup.closed.connect(function() { popup.destroy() })
        popup.open()
    } else {
        console.error("Lỗi tải DeletePopup:", comp.errorString())
    }
}

// ----------------------- CREATE NEW VAULT -----------------------
function openDynamicCreatePopup(sidebarRoot, vaultController) {
    let comp = Qt.createComponent("qrc:/View/SideBar/Popup/CreateVaultPopup.qml")

    if (comp.status === Component.Ready) {
        let popup = comp.createObject(sidebarRoot, {
                                          "primaryBlue": sidebarRoot.primaryBlue,
                                          "hoverBlue": sidebarRoot.hoverBlue,
                                          "pressBlue": sidebarRoot.pressBlue
                                      })

        popup.createConfirmed.connect(function(vName, vPass, vPath) {
            let success = vaultController.createVaultFolder(vName, vPass, vPath)

            if (success) {
                let fullVaultPath = vPath + "/" + vName
                sidebarRoot.vaultModel.append({
                                                  name: vName,
                                                  icon: "🔓",
                                                  password: vPass,
                                                  path: fullVaultPath
                                              })

                // UX: Tự động chọn và mở Vault vừa tạo
                let newIndex = sidebarRoot.vaultModel.count - 1;
                sidebarRoot.selectedVaultIndex = newIndex;
                vaultController.openVault(vName, vPass, fullVaultPath);
                sidebarRoot.vaultUnlocked(vName, fullVaultPath, vPass);

                // Đóng popup Create
                popup.close()

                // --- GỌI POPUP COPY KEK LÊN ---
                // 1. Gọi C++ lấy KEK dựa trên thông tin Vault vừa tạo
                let kekValue = listNodes.getVaultKEK(vName, vPass, fullVaultPath);

                // 2. Mở Popup hiển thị KEK
                let kekPopupComp = Qt.createComponent("qrc:/View/SideBar/Popup/CopyKEKPopup.qml");
                if (kekPopupComp.status === Component.Ready) {
                    let kekPopup = kekPopupComp.createObject(sidebarRoot, {
                                                                 "kekString": kekValue
                                                             });

                    // Lắng nghe tín hiệu khi user bấm "I have saved it securely"
                    kekPopup.copyConfirmed.connect(function() {
                        console.log("Người dùng đã xác nhận lưu KEK an toàn!");
                    });

                    kekPopup.open();
                } else {
                    console.error("Lỗi tải CopyKEKPopup:", kekPopupComp.errorString());
                }
                // ------------------------------

            } else {
                console.log("[ERROR]: Could not create a directory for Vault.")
            }
        })

        popup.closed.connect(function() { popup.destroy() })
        popup.open()
    } else {
        console.error("Lỗi tải CreateVaultPopup:", comp.errorString())
    }
}

//-----------------------  IMPORT VAULT -----------------------
function openDynamicImportPopup(sidebarRoot, vaultController) {
    let comp = Qt.createComponent("qrc:/View/SideBar/Popup/ImportVaultPopup.qml");

    if (comp.status === Component.Ready) {
        let popup = comp.createObject(sidebarRoot);

        popup.vaultImported.connect(function(folderPath, password) {
            // 1. Tách tên Vault từ đường dẫn (ví dụ: /home/data/MySecret -> MySecret)
            let pathParts = folderPath.split('/');
            let vaultName = pathParts[pathParts.length - 1];
            if (vaultName === "") vaultName = pathParts[pathParts.length - 2];

            console.log("Đang kiểm tra password cho Vault Import:", vaultName);

            // 2. Kiểm tra mật khẩu (Sử dụng hàm bạn đã thêm vào model.h)
            let isCorrect = listNodes.checkPassword(vaultName, password, folderPath);

            if (isCorrect) {
                // 3. Mật khẩu ĐÚNG -> Lưu vào PathDB (Gọi hàm C++ mới add ở Bước 1)
                let isSaved = vaultController.addExistingVault(vaultName, folderPath);

                if (isSaved) {
                    popup.showSuccess("Import successful! Opening vault...");

                    // 4. Tích hợp lên Vault Folder Section (Cập nhật ListModel)
                    sidebarRoot.vaultModel.append({
                                                      "name": vaultName,
                                                      "path": folderPath,
                                                      "password": password,
                                                      "icon": "🔓"
                                                  });

                    // Đợi 1 giây để user thấy thông báo xanh rồi đóng popup
                    let timer = Qt.createQmlObject("import QtQml 2.15; Timer {}", popup);
                    timer.interval = 1000;
                    timer.repeat = false;
                    timer.triggered.connect(function() {
                        popup.close();
                        timer.destroy();
                    });
                    timer.start();
                } else {
                    popup.showError("Vault already exists in list or Database error.");
                }
            } else {
                popup.showError("Incorrect password for this vault.");
            }
        });

        popup.closed.connect(function() { popup.destroy() });
        popup.open();
    }
}

//----------------------- UNCLOCK -----------------------
function openDynamicUnlockPopup(sidebarRoot, vaultController, vaultIndex) {
    if (vaultIndex === -1) return;

    let data = sidebarRoot.vaultModel.get(vaultIndex);
    let vaultName = data.name || data.vaultName;
    let vaultPath = data.path;

    let comp = Qt.createComponent("qrc:/View/SideBar/Popup/UnlockVaultPopup.qml");

    if (comp.status === Component.Ready) {
        let popup = comp.createObject(sidebarRoot, {
                                          "vaultName": vaultName
                                      });

        popup.unlockRequested.connect(function(password) {
            console.log("Đang kiểm tra mật khẩu qua NodeModel (listNodes)...");

            // 1. Kiểm tra mật khẩu trực tiếp qua model.h thay vì vaultController
            let isCorrect = listNodes.checkPassword(vaultName, password, vaultPath);

            if (isCorrect) {
                sidebarRoot.vaultModel.setProperty(vaultIndex, "password", password);
                sidebarRoot.vaultModel.setProperty(vaultIndex, "icon", "🔓");

                // ---- CẬP NHẬT LOGIC THÀNH CÔNG Ở ĐÂY ----
                console.log("[SUCCESS] Mật khẩu chính xác! Đang chuẩn bị mở Vault...");

                // Hiển thị chữ màu Xanh báo thành công trên Popup
                popup.showSuccess("Password accepted! Unlocking your files...");

                // Khởi tạo một Timer trễ 0.8 giây (800ms) để người dùng kịp đọc thông báo
                let timer = Qt.createQmlObject("import QtQml 2.15; Timer {}", popup);
                timer.interval = 1000;
                timer.repeat = false;
                timer.triggered.connect(function() {
                    // Hết 0.8s mới thực sự phát lệnh mở thư mục
                    vaultController.openVault(vaultName, password, vaultPath);
                    sidebarRoot.vaultUnlocked(vaultName, vaultPath, password);

                    // Tắt Popup và dọn dẹp bộ nhớ Timer
                    popup.close();
                    timer.destroy();
                });
                timer.start();

            } else {
                // Mật khẩu SAI -> Hiện thông báo lỗi đỏ trên Popup
                console.log("[ERROR] Sai mật khẩu truy cập.");
                popup.showError("Incorrect password. Please try again.");
            }
        });

        popup.closed.connect(function() { popup.destroy() });
        popup.open();
    } else {
        console.error("Lỗi tải UnlockVaultPopup:", comp.errorString());
    }
}

//----------------------- CHANGE ACCOUNT PASSWORD -----------------------
function openDynamicChangeAccountPasswordPopup(sidebarRoot, authManager) {
    let comp = Qt.createComponent("qrc:/View/SideBar/Popup/ChangeAccountPasswordPopup.qml");

    if (comp.status === Component.Ready) {
        let popup = comp.createObject(sidebarRoot);

        popup.changeAccountPasswordRequested.connect(function(currentPass, newPass) {
            console.log("Yêu cầu đổi mật khẩu cho Account...");

            // BƯỚC 1: Gọi C++ đổi mật khẩu Account
            let result = authManager.changeAccountPassword(currentPass, newPass);

            if (result.success) {
                popup.showSuccess("Account password changed successfully!");

                // Đợi 1 giây rồi đóng popup và MỞ POPUP COPY KEK
                let timer = Qt.createQmlObject("import QtQml 2.15; Timer {}", popup);
                timer.interval = 1000; timer.repeat = false;
                timer.triggered.connect(function() {
                    popup.close();
                    timer.destroy();

                    // Gọi component CopyAccountKEKPopup (LƯU Ý: Chỉnh đường dẫn qrc cho đúng với thư mục bạn đang lưu)
                    let kekComp = Qt.createComponent("qrc:/View/SignupView/CopyAccountKEKPopup.qml");
                    if (kekComp.status === Component.Ready) {
                        let kekPopup = kekComp.createObject(sidebarRoot, {
                                                                "kekString": result.newKek
                                                            });
                        kekPopup.copyConfirmed.connect(function() {
                            kekPopup.close();
                        });
                        kekPopup.open();
                    } else {
                        console.error("Lỗi tải CopyAccountKEKPopup:", kekComp.errorString());
                    }
                });
                timer.start();
            } else {
                popup.showError("Incorrect current password or database error.");
            }
        });

        popup.closed.connect(function() { popup.destroy() });
        popup.open();
    } else {
        console.error("Lỗi tải ChangeAccountPasswordPopup:", comp.errorString());
    }
}

//-----------------------  CHANGE VAULT PASSWORD -----------------------
function openDynamicChangeVaultPasswordPopup(sidebarRoot, vaultController) {
    if (sidebarRoot.selectedVaultIndex === -1) return;

    let data = sidebarRoot.vaultModel.get(sidebarRoot.selectedVaultIndex);
    let vName = data.name;
    let vPath = data.path;

    let comp = Qt.createComponent("qrc:/View/SideBar/Popup/ChangeVaultPasswordPopup.qml");

    if (comp.status === Component.Ready) {
        let popup = comp.createObject(sidebarRoot, { "vaultName": vName });

        popup.changeVaultPasswordRequested.connect(function(currentPass, newPass) {
            console.log("Yêu cầu đổi mật khẩu cho Vault:", vName);

            // BƯỚC 1: Kiểm tra mật khẩu cũ có đúng không (Dùng hàm listNodes.checkPassword)
            let isCorrect = listNodes.checkPassword(vName, currentPass, vPath);

            if (isCorrect) {
                // BƯỚC 2: Gọi C++ thực thi lệnh đổi mật khẩu và NHẬN VỀ KEK MỚI
                let newKekString = vaultController.changeVaultPassword(vName, vPath, currentPass, newPass);

                // KIỂM TRA NẾU KEK KHÔNG RỖNG (TỨC LÀ THÀNH CÔNG)
                if (newKekString !== "") {
                    popup.showSuccess("Password changed successfully!");

                    // Cập nhật lại mật khẩu mới vào listModel
                    sidebarRoot.vaultModel.setProperty(sidebarRoot.selectedVaultIndex, "password", "");

                    // 2. Đổi icon về lại ổ khóa đang đóng
                    sidebarRoot.vaultModel.setProperty(sidebarRoot.selectedVaultIndex, "icon", "🔐");

                    // 3. Gửi tín hiệu rỗng để MainContent ẩn danh sách file hiện tại đi
                    sidebarRoot.vaultUnlocked("", "", "");

                    // Đợi 1 giây rồi đóng popup và MỞ POPUP COPY KEK
                    let timer = Qt.createQmlObject("import QtQml 2.15; Timer {}", popup);
                    timer.interval = 1000; timer.repeat = false;
                    timer.triggered.connect(function() {
                        popup.close();
                        timer.destroy();

                        // COPY KEK POPUP
                        openCopyKEKPopup(sidebarRoot, newKekString);
                    });
                    timer.start();
                } else {
                    popup.showError("Database Error: Failed to change password.");
                }
            } else {
                popup.showError("Incorrect current password.");
            }
        });

        popup.closed.connect(function() { popup.destroy() });
        popup.open();
    }
}

//--------- FORGOT VAULT PASSWORD -------------
function openDynamicForgetPasswordPopup(sidebarRoot, vaultController) {
    if (sidebarRoot.selectedVaultIndex === -1) return;

    let data = sidebarRoot.vaultModel.get(sidebarRoot.selectedVaultIndex);
    let vName = data.name;
    let vPath = data.path;

    let comp = Qt.createComponent("qrc:/View/SideBar/Popup/ForgotPasswordPopup.qml");

    if (comp.status === Component.Ready) {
        let popup = comp.createObject(sidebarRoot, { "vaultName": vName });

        // Bước 1: Kiểm tra KEK (Verify)
        popup.verifyKekRequested.connect(function(kek) {
            // Gọi C++ để kiểm tra xem KEK này có mở được DB không
            // Giả sử bạn có hàm: vaultController.verifyVaultKEK(vName, vPath, kek)
            let isValid = vaultController.verifyVaultKEK(vName, vPath, kek);
            if (isValid) {
                popup.isKekVerified = true;
            } else {
                console.log("KEK không hợp lệ!");
                // Bạn có thể thêm hàm showError vào Popup tương tự như ChangePassword
            }
        });

        // Bước 2: Thực hiện Reset mật khẩu
        popup.resetPasswordRequested.connect(function(kek, newPass) {
            // Gọi C++ để đổi mật khẩu dựa trên KEK cũ
            let result = vaultController.resetVaultPasswordWithKEK(vName, vPath, kek, newPass);

            if (result.success) {
                // 1. Xóa password trong Model để Vault chuyển về trạng thái Locked
                sidebarRoot.vaultModel.setProperty(sidebarRoot.selectedVaultIndex, "password", "");

                // 2. Đổi icon về lại ổ khóa đang đóng
                sidebarRoot.vaultModel.setProperty(sidebarRoot.selectedVaultIndex, "icon", "🔐");

                // 3. Gửi tín hiệu rỗng để MainContent ẩn danh sách file hiện tại đi
                sidebarRoot.vaultUnlocked("", "", "");
                popup.close();
                // Hiện popup KEK mới để người dùng backup lại
                openCopyKEKPopup(sidebarRoot, result.newKek);
            }
        });

        popup.closed.connect(function() { popup.destroy() });
        popup.open();
    }
}

// Hàm bổ trợ để mở Popup Copy KEK
function openCopyKEKPopup(sidebarRoot, newKek) {
    let comp = Qt.createComponent("qrc:/View/SideBar/Popup/CopyKEKPopup.qml");
    if (comp.status === Component.Ready) {
        let kekPopup = comp.createObject(sidebarRoot, { "kekString": newKek });
        kekPopup.open();
    }
}
