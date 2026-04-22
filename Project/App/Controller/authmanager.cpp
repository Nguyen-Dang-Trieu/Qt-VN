#include "authmanager.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include "logger.h"

CAuthManager::CAuthManager(CPathDB *pathDB, QObject *parent)
    : QObject(parent), m_pathDB(pathDB) {}

// =================================================================
// HÀM KIỂM TRA TỒN TẠI TÀI KHOẢN
// =================================================================
bool CAuthManager::isAccountExists(const QString &username) {
    QString dbPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.cryptvault/" + username + "/path.db";
    return QFile::exists(dbPath);

}

// =================================================================
// SIGN UP
// =================================================================
bool CAuthManager::handleSignUp(const QString &username, const QString &password) {
    if (!m_pathDB) return false;

    QString homePath      = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QString hiddenDirPath = homePath + "/.cryptvault/" + username;
    QString dbPath        = hiddenDirPath + "/user_data.db";

    // BẢO VỆ 1: Tránh ghi đè nếu tài khoản đã tồn tại
    if (QFile::exists(dbPath)) {
        LOG_ERROR("Tài khoản '" + username + "' đã tồn tại! Vui lòng đăng nhập.");
        return false;
    }

    // BẢO VỆ 2: Tạo thư mục ẩn riêng cho user nếu chưa có
    QDir dir;
    if (!dir.exists(hiddenDirPath)) {
        if (dir.mkpath(hiddenDirPath)) { // mkpath sẽ tự động tạo luôn cả .cryptvault nếu chưa có
            LOG_INFO("Created user directory at: " + hiddenDirPath);
        } else {
            LOG_ERROR("Could not create directory for user: " + username);
            return false;
        }
    }

    m_pathDB->setUsernameSignup(username);
    m_pathDB->setPasswordSignup(password);

    // 2. Mở DB. Do file chưa tồn tại, SQLCipher sẽ TẠO MỚI file và mã hóa bằng KEK.
    if (m_pathDB->open(dbPath)) {
        LOG_INFO("Đăng ký & Khởi tạo DB THÀNH CÔNG cho user: " + username);
        return true;
    }

    LOG_ERROR("Lỗi: Không thể khởi tạo file DB khi đăng ký.");
    return false;
}

// =================================================================
// LOGIN
// =================================================================
bool CAuthManager::handleLogin(const QString &username, const QString &password) {
    if (!m_pathDB) return false;

    QString homePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);

    // CẬP NHẬT: Trỏ đường dẫn DB vào đúng thư mục của user đang cố gắng đăng nhập
    QString dbPath   = homePath + "/.cryptvault/" + username + "/user_data.db";

    // KIỂM TRA NHANH: Nếu chưa có file DB trong thư mục user này
    if (!QFile::exists(dbPath)) {
        LOG_ERROR("Tài khoản '" + username + "' không tồn tại! Vui lòng Đăng ký trước.");
        return false;
    }

    // 1. Set 2 giá trị vào PathDB để hệ thống ngầm tính toán KEK
    m_pathDB->setUsernameSignup(username);
    m_pathDB->setPasswordSignup(password);

    // 2. Mở DB. PathDB sẽ dùng KEK vừa tính để thử giải mã file user_data.db
    if (m_pathDB->open(dbPath)) {
        LOG_INFO("Đăng nhập THÀNH CÔNG cho: " + username);
        return true;
    }

    LOG_ERROR("Đăng nhập THẤT BẠI: Sai Mật khẩu cho tài khoản " + username + "!");
    return false;
}

QString CAuthManager::getActiveUsername() {
    if (m_pathDB) {
        return m_pathDB->getUsernameSignup();
    }
    return "Vault User";
}

// add start 20260420
bool CAuthManager::verifyAccountKEK(const QString &username, const QString &kekHex) {
    if (!m_pathDB) return false;

    QString homePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QString dbPath   = homePath + "/.cryptvault/" + username + "/user_data.db";

    if (!QFile::exists(dbPath)) {
        LOG_ERROR("Tài khoản '" + username + "' không tồn tại!");
        return false;
    }

    // Gọi hàm mở bằng KEK vừa tạo
    if (m_pathDB->openWithKEK(dbPath, kekHex)) {
        LOG_INFO("Xác thực KEK THÀNH CÔNG cho Account: " + username);
        return true;
    }

    LOG_ERROR("Xác thực KEK THẤT BẠI cho Account: " + username);
    return false;
}

QJsonObject CAuthManager::resetAccountPasswordWithKEK(const QString &username, const QString &kekHex, const QString &newPass) {
    LOG_INFO("Yêu cầu reset mật khẩu Account: " + username);

    QJsonObject result;
    result["success"] = false;
    result["newKek"] = "";

    if (!m_pathDB) return result;

    QString homePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QString dbPath   = homePath + "/.cryptvault/" + username + "/user_data.db";
    m_pathDB->setUsernameSignup(username);

    if (!m_pathDB->openWithKEK(dbPath, kekHex)) {
        LOG_ERROR("KEK không hợp lệ, không thể reset mật khẩu.");
        return result;
    }

    if (m_pathDB->changePassword(newPass)) {
        LOG_INFO("Reset mật khẩu Account thành công!");
        result["success"] = true;
        result["newKek"]  = m_pathDB->getKEK();
    } else {
        LOG_ERROR("Lỗi trong quá trình Rekey PathDB.");
    }

    m_pathDB->close();
    return result;
}

QJsonObject CAuthManager::changeAccountPassword(const QString &currentPass, const QString &newPass) {
    QJsonObject result;
    result["success"] = false;
    result["newKek"] = "";

    if (!m_pathDB) return result;

    if (m_pathDB->getPasswordSignup() != currentPass) {
        LOG_ERROR("Sai mật khẩu Account hiện tại.");
        return result;
    }

    if (m_pathDB->changePassword(newPass)) {
        LOG_INFO("Đổi mật khẩu Account thành công!");
        result["success"] = true;
        result["newKek"]  = m_pathDB->getKEK(); // Trả về KEK mới
    } else {
        LOG_ERROR("Lỗi trong quá trình Rekey PathDB.");
    }

    return result;
}
// add end 20260420
