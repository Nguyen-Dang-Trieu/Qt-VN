#include "vaultcontroller.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QUrl>
#include <QDebug>

VaultController::VaultController(QObject *parent) : QObject(parent) {}

bool VaultController::copyPathToVault(QString sourceUrl, QString vaultName) {
    // 1. Lấy đường dẫn sạch để không bị phụ thuộc vào từng loại hệ điều hành khác nhau
    QString cleanSource = QUrl(sourceUrl).toLocalFile();
    if (cleanSource.isEmpty()) return false;

    // 2. Định nghĩa RootDir (Đảm bảo có dấu / ở cuối)
    QString rootPath = "/home/GhostRider/Work/test_Qt/output_app/";

    // Tạo đường dẫn đầy đủ đến thư mục 'vault' bên trong folder của VaultName
    // Ví dụ: D:/Qt/Work/Lean_day_Qt/output_app/..../vault/
    QString fullDirPath = rootPath + vaultName + "/vault/";

    // 3. Kiểm tra thư mục đã có / chưa -> If chưa -> tạo thư mục
    QDir dir;
    if (!dir.exists(fullDirPath)) {
        // mkpath sẽ tạo tất cả thư mục trung gian nếu chúng chưa tồn tại
        if (dir.mkpath(fullDirPath)) {
            qDebug() << "[INFO] Đã tạo thành công thư mục:" << fullDirPath;
        } else {
            qDebug() << "[ERROR] Không thể tạo thư mục!";
            return false;
        }
    }

    // 4. Xác định tên file và đường dẫn đích cuối cùng
    QFileInfo sourceInfo(cleanSource); // cleanSource = D:/Qt/Work/Lean_day_Qt/output_app/ -> sourceInfo.fileName() = output_app
    QString destinationPath = fullDirPath + sourceInfo.fileName();

    qDebug() << "\n--- TIEN HANH COPY ---";
    qDebug() << "[FROM] " << cleanSource;
    qDebug() << "[TO]   " << destinationPath;

    // 5. Gọi hàm đệ quy copy
    bool success = copyRecursively(cleanSource, destinationPath);

    return success;
}

// Hàm đệ quy thần thánh để copy File lẫn Folder
bool VaultController::copyRecursively(const QString &srcFilePath, const QString &targetFilePath)
{
    /*
    srcFilePath    = C:/Nguon/A
        + Chứa secret.txt, LuongThang12.txt
    targetFilePath = D:/Dich/A
    */

    QFileInfo srcFileInfo(srcFilePath);

    // NẾU LÀ THƯ MỤC
    if (srcFileInfo.isDir()) {
        QDir targetDir(targetFilePath); // Ex: targetFilePath = D:/Dich/A -> targetDir = D:/Dich/A
        targetDir.cdUp();     // -> D:/Dich/

        // QFileInfo(targetFilePath).fileName() = "A"
        if (!targetDir.mkdir(QFileInfo(targetFilePath).fileName())) {
            // Tạo thành công thư mục A
        }

        /* Tạo một đối tượng thư mục của nguồn để thao tác với thư mục */
        QDir sourceDir(srcFilePath);

        // Danh sách file của thư mục nguồn
        QStringList listFile = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
        // listFile chua secret.txt va LuongThang12.txt

        foreach (const QString &fileName, listFile) {
            const QString newSrcFilePath = srcFilePath + "/" + fileName;    // Ex: newSrcFilePath = "C:/Nguon/A" + "/" + "secret.txt"
            const QString newTgtFilePath = targetFilePath + "/" + fileName; // Ex: newTgtFilePath = "D:/Dich/A" + "/" + "secret.txt"
            if (!copyRecursively(newSrcFilePath, newTgtFilePath)) {
                return false;
            }
        }
    }
    // NẾU LÀ FILE
    else {
        // Ghi đè nếu file đã tồn tại
        if (QFile::exists(targetFilePath)) {
            QFile::remove(targetFilePath);
        }
        if (!QFile::copy(srcFilePath, targetFilePath)) {
            return false;
        }
    }
    return true;
}