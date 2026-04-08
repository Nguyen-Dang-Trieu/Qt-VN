#include "vaultcontroller.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QUrl>
#include <QDebug>

VaultController::VaultController(QObject *parent) : QObject(parent) {}

bool VaultController::copyPathToVault(QString sourceUrl, QString vaultName) {
    // 1. Lấy đường dẫn sạch
    QString cleanSource = QUrl(sourceUrl).toLocalFile();
    if (cleanSource.isEmpty()) return false;

    // 2. Định nghĩa RootDir (Đảm bảo có dấu / ở cuối)
    QString rootPath = "D:/Qt/Work/Lean_day_Qt/output_app/";

    // Tạo đường dẫn đầy đủ đến thư mục 'vault' bên trong folder của VaultName
    // Ví dụ: D:/Qt/Work/Lean_day_Qt/output_app/MyVault/vault/
    QString fullDirPath = rootPath + vaultName + "/vault/";

    // 3. ÉP BUỘC TẠO THƯ MỤC (Cực kỳ quan trọng)
    QDir dir;
    if (!dir.exists(fullDirPath)) {
        // mkpath sẽ tạo tất cả thư mục trung gian nếu chúng chưa tồn tại
        if (dir.mkpath(fullDirPath)) {
            qDebug() << "[INFO] Da tao thanh cong chuoi thu muc:" << fullDirPath;
        } else {
            qDebug() << "[ERROR] Khong the tao thu muc. Kiem tra quyen ghi o o D:";
            return false;
        }
    }

    // 4. Xác định tên file và đường dẫn đích cuối cùng
    QFileInfo sourceInfo(cleanSource);
    QString destinationPath = fullDirPath + sourceInfo.fileName();

    qDebug() << "--- TIEN HANH COPY ---";
    qDebug() << "Tu:" << cleanSource;
    qDebug() << "Den:" << destinationPath;

    // 5. Gọi hàm đệ quy copy
    bool success = copyRecursively(cleanSource, destinationPath);

    return success;
}

// Hàm đệ quy thần thánh để copy File lẫn Folder
bool VaultController::copyRecursively(const QString &srcFilePath, const QString &tgtFilePath)
{
    QFileInfo srcFileInfo(srcFilePath);

    // NẾU LÀ THƯ MỤC
    if (srcFileInfo.isDir()) {
        QDir targetDir(tgtFilePath);
        targetDir.cdUp();
        if (!targetDir.mkdir(QFileInfo(tgtFilePath).fileName())) {
            // Thư mục đã tồn tại, tiếp tục chui vào trong
        }

        QDir sourceDir(srcFilePath);
        QStringList fileNames = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);

        foreach (const QString &fileName, fileNames) {
            const QString newSrcFilePath = srcFilePath + "/" + fileName;
            const QString newTgtFilePath = tgtFilePath + "/" + fileName;
            if (!copyRecursively(newSrcFilePath, newTgtFilePath)) {
                return false;
            }
        }
    }
    // NẾU LÀ FILE
    else {
        // Ghi đè nếu file đã tồn tại
        if (QFile::exists(tgtFilePath)) {
            QFile::remove(tgtFilePath);
        }
        if (!QFile::copy(srcFilePath, tgtFilePath)) {
            return false;
        }
    }
    return true;
}
