#include <QObject>
#include <QDir>
#include <QDebug>
#include <QStandardPaths>
#include "vaultcontroller.h"

//VaultController::VaultController(QObject *parent)
//    : QObject(parent)
//{
//}

//bool VaultController::createVaultFolder(const QString &vaultName, const QString &password) {
//    if (vaultName.isEmpty() || password.isEmpty()) return false;

//    // 1. Create Path to Vault Folder
//    QString RootDir     = "D:\Qt\Work\output_app\";
//    QString basePath    = RootDir + vaultName;
//    QString dataDirPath = basePath + "/vault";
//    QString dbPath      = basePath + "/" + vaultName + ".db";

//    // 2. Create directory struct
//    QDir dir;
//    if (!dir.mkpath(dataDirPath)) {
//        LOG_ERROR("[CONTROLLER] Don't create 'Data Dir'" + dataDirPath);
//        return false;
//    }

//    // 3. Init DB by VaultDB
//    VaultDB vDB;

//    if (!vDB.open(dbPath, password)) {
//        LOG_ERROR("Open DB Failed!");
//        return false;
//    }

//    if (!vDB.migrate()) {
//        LOG_ERROR("Migrate Database Failed!");
//        return false;
//    }

//    // Gán database cho node
//    vDB.node.setDb(&vDB._db_);

//    if (vDB.node.count("") == 0) {
//        LOG_INFO("Initializing Root directory in Database...");

//        Node root;
//        root.m_name = "Root";
//        root.m_type = 1;
//        root.m_parent = 0;
//        root.m_time = QDateTime::currentSecsSinceEpoch();

//        if (vDB.node.insert(root)) {
//            LOG_INFO("Root Node created successfully with ID: " + QString::number(root.m_id));
//        } else {
//            LOG_ERROR("Failed to create Root Node!");
//        }
//    }


//    LOG_INFO("DB is ready to USE!");
//    vDB.close();

//    return true;
//}


//bool VaultController::copyPathToVault(QString sourcePath, QString vaultName, QString password) {
//    QString cleanSource = sourcePath;
//    if (cleanSource.startsWith("file://")) {
//        cleanSource = QUrl(sourcePath).toLocalFile();
//    }

//    QString RootDir = "/home/GhostRider/Work/clone-app-vault/output_app/";
//    QString basePath = RootDir + vaultName;
//    QString destinationDir = basePath + "/vault/";
//    QString dbPath = basePath + "/" + vaultName + ".db";

//    VaultDB vDB;
//    if (!vDB.open(dbPath, password)) {
//        LOG_ERROR("Không thể mở Vault DB để đồng bộ dữ liệu!");
//        return false;
//    }

//    vDB.node.setDb(&vDB._db_);

//    QVector<Node> roots = vDB.node.query(" select * from node WHERE name = 'Root' AND type = 1 LIMIT 1");
//    if (roots.isEmpty()) {
//        LOG_ERROR("Không tìm thấy Root Node trong Database!");
//        roots = vDB.node.query("LIMIT 1");
//    }

//    if (roots.isEmpty()) {
//        LOG_ERROR("Database hoàn toàn trống! Không thể copy.");
//        vDB.close();
//        return false;
//    }

//    int rootId = roots[0].m_id;

//    // 5. Bắt đầu đệ quy copy
//    QFileInfo srcInfo(cleanSource);
//    QString destinationPath = destinationDir + srcInfo.fileName();

//    bool success = copyRecursively(cleanSource, destinationPath, vDB, rootId);
//    if (success) {
//        LOG_INFO("Đã copy và đồng bộ Database thành công cho: " + srcInfo.fileName());
//    }

//    vDB.close();
//    return success;
//}

//bool VaultController::copyRecursively(const QString &srcFilePath, const QString &targetFilePath, VaultDB &vDB, int parentId)
//{
//    QFileInfo srcFileInfo(srcFilePath);

//    // Chuẩn bị object Node để chuẩn bị insert vào DB
//    Node newNode;
//    newNode.m_name = srcFileInfo.fileName();
//    newNode.m_parent = parentId;
//    newNode.m_time = QDateTime::currentSecsSinceEpoch();

//    // --- TRƯỜNG HỢP LÀ THƯ MỤC ---
//    if (srcFileInfo.isDir()) {
//        QDir targetDir(targetFilePath);
//        targetDir.cdUp();
//        if (!targetDir.mkdir(srcFileInfo.fileName())) {
//        }

//        // 2. Insert Folder vào Database
//        newNode.m_type = 1; // 1: Folder
//        if (!vDB.node.insert(newNode)) {
//            LOG_ERROR("DB Error: Không thể chèn Folder " + newNode.m_name);
//            return false;
//        }

//        // 3. Duyệt các file con bên trong
//        QDir sourceDir(srcFilePath);
//        QStringList listFile = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

//        foreach (const QString &fileName, listFile) {
//            const QString newSrc = srcFilePath + "/" + fileName;
//            const QString newTgt = targetFilePath + "/" + fileName;

//            if (!copyRecursively(newSrc, newTgt, vDB, newNode.m_id)) {
//                return false;
//            }
//        }
//    }
//    // --- TRƯỜNG HỢP LÀ FILE ---
//    else {
//        // 1. Copy file vật lý
//        if (QFile::exists(targetFilePath)) {
//            QFile::remove(targetFilePath);
//        }
//        if (!QFile::copy(srcFilePath, targetFilePath)) {
//            LOG_ERROR("Disk Error: Không thể copy file tới " + targetFilePath);
//            return false;
//        }

//        // 2. Insert File vào Database
//        newNode.m_type = 2; // 2: File
//        newNode.m_length = srcFileInfo.size();
//        if (!vDB.node.insert(newNode)) {
//            LOG_ERROR("DB Error: Không thể chèn File " + newNode.m_name);
//            return false;
//        }
//    }

//    return true;
//}
