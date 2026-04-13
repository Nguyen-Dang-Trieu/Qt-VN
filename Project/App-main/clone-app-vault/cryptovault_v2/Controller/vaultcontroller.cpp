#include <openssl/rand.h>
#include "vaultcontroller.h"
#include "VaultDB.h"
#include "logger.h"
#include "support.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>


VaultController::VaultController(QObject *parent)
    : QObject(parent)
{
    pathDB = new CPathDB(this);
}

bool VaultController::createVaultFolder(const QString &vaultName, const QString &password, const QString &path) {
    if (vaultName.isEmpty() || password.isEmpty() || path.isEmpty()) return false;

    QString basePath    = path + "/" + vaultName;
    QString dataDirPath = basePath + "/vault";
    QString dbPath      = basePath + "/" + vaultName + ".db";

    QDir dir;
    if (!dir.mkpath(dataDirPath)) {
        LOG_ERROR("[CONTROLLER] Don't create 'Data Dir' " + dataDirPath);
        return false;
    }

    VaultDB vDB;
    bool isDbReady = vDB.open(dbPath, password);
    if (isDbReady) {
        LOG_INFO("Vault DB created successfully at: " + basePath);
        pathDB->saveVaultFolder(vaultName, basePath);
    } else {
        LOG_ERROR("An Error ocurred when init db for folder vault!");
        return false;
    }

    if (!vDB.migrate()) {
        LOG_ERROR("Migrate Database Failed!");
        return false;
    }

    /* ---------------------------------------- Them thong tin vault va masterkey vao trong Table Vault config --- */
    // Random masterkey
    QByteArray MasterKey(32, 0); // 32 bytes
    MasterKey = randomKey();

    vDB.vaultConfig.setDb(&vDB._db_);
    VaultConfig infoVault; // Thong tin cua vault dang tao
    infoVault.name = vaultName;
    infoVault.masterKey = MasterKey.toHex();
    infoVault.version   = 1;
    vDB.vaultConfig.insert(infoVault);

    LOG_INFO("MasterKey: " + MasterKey.toHex());

    /* ------------------------------------------ Tao Node Root cho table node --- */
    vDB.node.setDb(&vDB._db_);

    if (vDB.node.count("") == 0) {
        LOG_INFO("Initializing Root directory in Database...");

        Node root;
        root.m_name = "Root";
        root.m_type = 1;
        root.m_parent = 0;
        root.m_time = QDateTime::currentSecsSinceEpoch();

        if (vDB.node.insert(root)) {
            LOG_INFO("Root Node created successfully with ID: " + QString::number(root.m_id));
        } else {
            LOG_ERROR("Failed to create Root Node!");
        }
    }

    LOG_INFO("DB is ready to USE!");
    vDB.close();

    return true;
}


QVariantList VaultController::loadVaults() {
    return pathDB->loadVaultFolders();
}

bool VaultController::deleteVault(const QString &vaultPath) {
    // 1. Xóa đường dẫn khỏi Database
    bool dbSuccess = pathDB->deleteVaultFolder(vaultPath);

    if (dbSuccess) {
        QDir dir(vaultPath);
        if (dir.exists()) {
            dir.removeRecursively();
        }
    }

    return dbSuccess;
}

QString VaultController::renameVault(const QString &oldPath, const QString &newName) {
    QString cleanOldPath = oldPath;
    if (cleanOldPath.startsWith("file://")) {
        cleanOldPath = QUrl(oldPath).toLocalFile();
    }

    QDir dir(cleanOldPath);

    // 2. Kiểm tra thư mục cũ có thực sự tồn tại trên ổ cứng không
    if (!dir.exists()) {
        LOG_ERROR("HỦY ĐỔI TÊN: Thư mục gốc không tồn tại: " + cleanOldPath);
        return "";
    }

    QFileInfo fileInfo(cleanOldPath);
    QString parentDir = fileInfo.absolutePath();
    QString newPath = parentDir + "/" + newName;

    // 3. Kiểm tra xem tên mới đã bị trùng với một thư mục khác chưa
    if (QDir(newPath).exists()) {
        LOG_WARNING("HỦY ĐỔI TÊN: Tên Vault mới đã tồn tại trên ổ đĩa: " + newPath);
        return "";
    }

    LOG_INFO("Đang ra lệnh Hệ điều hành đổi tên: " + cleanOldPath + " ---> " + newPath);

    // 4. Thực thi đổi tên trên ổ cứng
    QDir rootDir;
    if (rootDir.rename(cleanOldPath, newPath)) {

        // 5. Cập nhật vào SQLite
        if (pathDB->updateVaultFolder(cleanOldPath, newPath)) {
            LOG_INFO("✅ Đổi tên Vault thành công toàn diện!");
            return newPath;
        } else {
            // Rollback: Nếu DB lỗi thì trả lại tên thư mục cũ để không bị lệch data
            rootDir.rename(newPath, cleanOldPath);
            LOG_ERROR("Lỗi Database: Đã rollback (khôi phục) tên thư mục.");
        }
    } else {
        LOG_ERROR("❌ Hệ điều hành từ chối quyền đổi tên thư mục!");
    }
    return "";
}

/**
 * @brief Them file or folder vao vault theo path
 * @param sourcePath URL cua file / folder can ma hoa
 * @param vaultName  Chua du lieu ma hoa
 * @param password
 * @return
 */
bool VaultController::importDataToVault(QString sourcePath, QString vaultName, QString password) {
    QString cleanSource = sourcePath;
    if (cleanSource.startsWith("file://")) {
        cleanSource = QUrl(sourcePath).toLocalFile();
    }

    LOG_WARNING("Path vault is selected: " + pathDB->getVaultPathByName(vaultName));

    QString basePath = pathDB->getVaultPathByName(vaultName);
    QString destinationDir = basePath + "/vault/";
    QString dbPath = basePath + "/" + vaultName + ".db";

    LOG_DEBUG("dbPath: " + dbPath);
    VaultDB vDB;
    if (!vDB.open(dbPath, password)) {
        LOG_ERROR("Không thể mở Vault DB để đồng bộ dữ liệu!");
        return false;
    }

    vDB.node.setDb(&vDB._db_);

    QVector<Node> roots = vDB.node.query(QString("select * from node WHERE name = 'Root' AND type = 1 LIMIT 1"));
    if (roots.isEmpty()) {
        LOG_ERROR("Database hoàn toàn trống! Không thể copy.");
        vDB.close();
        return false;
    }

    int rootId = roots[0].m_id;

    // 5. Bắt đầu đệ quy copy
    QFileInfo srcInfo(cleanSource);
    QString destinationPath = destinationDir + srcInfo.fileName();

    bool success = copyRecursively(cleanSource, destinationPath, vDB, rootId);

    if (!success) {
        LOG_INFO("Copy Failed:  " + srcInfo.fileName());
    }

    vDB.close();
    return success;
}

bool VaultController::copyRecursively(const QString &srcFilePath, const QString &targetFilePath, VaultDB &vDB, int parentId)
{
    QFileInfo srcFileInfo(srcFilePath);

    // Chuẩn bị object Node để chuẩn bị insert vào DB
    Node newNode;
    newNode.m_name = srcFileInfo.fileName();
    newNode.m_parent = parentId;
    newNode.m_time = QDateTime::currentSecsSinceEpoch();

    // --- TRƯỜNG HỢP LÀ THƯ MỤC ---
    if (srcFileInfo.isDir()) {
        QDir targetDir(targetFilePath);
        targetDir.cdUp();
        if (!targetDir.mkdir(srcFileInfo.fileName())) {
        }

        // 2. Insert Folder vào Database
        newNode.m_type = 1; // 1: Folder
        if (!vDB.node.insert(newNode)) {
            LOG_ERROR("DB Error: Không thể chèn Folder " + newNode.m_name);
            return false;
        }

        // 3. Duyệt các file con bên trong
        QDir sourceDir(srcFilePath);
        QStringList listFile = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

        foreach (const QString &fileName, listFile) {
            const QString newSrc = srcFilePath + "/" + fileName;
            const QString newTgt = targetFilePath + "/" + fileName;

            if (!copyRecursively(newSrc, newTgt, vDB, newNode.m_id)) {
                return false;
            }
        }
    }
    // --- TRƯỜNG HỢP LÀ FILE ---
    else {
        // 1. Copy file vật lý
        if (QFile::exists(targetFilePath)) {
            QFile::remove(targetFilePath);
        }
        if (!QFile::copy(srcFilePath, targetFilePath)) {
            LOG_ERROR("Disk Error: Không thể copy file tới " + targetFilePath);
            return false;
        }

        // 2. Insert File vào Database
        newNode.m_type = 2; // 2: File
        newNode.m_length = srcFileInfo.size();
        if (!vDB.node.insert(newNode)) {
            LOG_ERROR("DB Error: Không thể chèn File " + newNode.m_name);
            return false;
        }

        // 3. Lay ID cua node file vua duoc them vao
        QString query = QString("WHERE name = '%1' LIMIT 1").arg(srcFileInfo.fileName().replace("'", "''"));
        QVector<Node> results = vDB.node.fetch(query);

        quint64 id_file = results.first().m_id; // lay id cua node dau tien

        QString vaultDataDir = QFileInfo(targetFilePath).absolutePath();
        ingestFileToBuckets(vDB, id_file, srcFilePath, vaultDataDir, "empty_key");
    }

    return true;
}

bool VaultController::ingestFileToBuckets(VaultDB &vDB, quint64 nodeID,
                                         QString pathSrcFile,
                                         QString pathOfVaultFolder,
                                         QString password)
{
    vDB.nodeData.setDb(&vDB._db_);
    vDB.bucket.setDb(&vDB._db_);

    /* ----Tim bucket ---- */
    quint64 bucketID = ensureAvailableBucket(vDB, pathOfVaultFolder);
    if (bucketID == 0) {
        LOG_ERROR("It is not possible to obtain or create Bucket!");
        return false;
    }

    // Mo file Bucket
    QString query = QString("WHERE id = '%1' LIMIT 1").arg(bucketID);
    QVector<Bucket> results = vDB.bucket.fetch(query);
    if(results.isEmpty()) {
        LOG_ERROR("No bucket found with ID: " + QString::number(bucketID));
        return false;
    }

    Bucket &targetBucket = results.first();
    LOG_DEBUG("targetBucket id:" + QString::number(targetBucket.id));


    QString pathOfBucket = results.first().relative_path;
    QByteArray slicesRaw    = results.first().slices;
    // Test update slices

    QJsonDocument doc = QJsonDocument::fromJson(targetBucket.slices);
    QJsonArray    arrSlices;
    if(doc.isArray()) {
        arrSlices = doc.array();
    }

    QJsonObject newObj;
    newObj["slice_uuid"] = "test_";
    newObj["offset"]     = 0;

    arrSlices.append(newObj);

    QJsonDocument newDoc(arrSlices);
    targetBucket.slices = newDoc.toJson(QJsonDocument::Compact);

    if(vDB.bucket.save(targetBucket)) {
        LOG_DEBUG("update table bucket successful");
    }

    // LOG_DEBUG(pathOfBucket);
    QFile bucketFile(pathOfBucket);
    if(!bucketFile.open(QIODevice::Append | QIODevice::WriteOnly)) {
        LOG_ERROR("Failt to open Bucket File!" + pathOfBucket);
        return false;
    }

    /* --- File can luu tru --- */
    QFile     srcFile(pathSrcFile);         // ops of file
    QFileInfo srcFileInfo(pathSrcFile);     // info of file
    if(!srcFile.open(QIODevice::ReadOnly)) {
        LOG_ERROR("Unable to open source file: " + srcFileInfo.fileName());
        return false;
    }

    /* --- Read and Write data --- */
    quint64 slice_number = 0;
    QByteArray chunk;
    while (!(chunk = srcFile.read(CHUNK_SIZE)).isEmpty()) {
        QString sliceUuid   = srcFileInfo.completeBaseName() + "-" + generateRandomLetters();

        if(chunk.size() < CHUNK_SIZE) {
            chunk.append(CHUNK_SIZE - chunk.size(), '0');
        }
        // Ghi vào file
        bucketFile.write(chunk);

        // Kiem tra bucket file dat toi gioi han hay chua
        // .. Todo

        // Insert vao table node_data
        NodeData curr;
        curr.node_id      = nodeID;
        curr.bucket_id    = bucketID;
        curr.slice_uuid   = sliceUuid;
        curr.slice_number = slice_number;
        if (!vDB.nodeData.insert(curr)) {
            LOG_ERROR("Cannot add slice to node_data table!");
            return false;
        }

        slice_number++;
    }
    bucketFile.close();

////    // 6. Kiểm tra nếu bucket đã đầy (Ví dụ giới hạn 100MB)
////    const qint64 MAX_BUCKET_SIZE = 100 * 1024 * 1024;
////    if (QFileInfo(fullPath).size() >= MAX_BUCKET_SIZE) {
////        vDB.bucket.is_full = 1;
////        vDB.bucket.update(); // Cập nhật trạng thái vào DB
////    }

    return true;
}

/**
 * @brief Tim kiem file bucket phu hop (bucket is not full)
 * @param vDB
 * @param pathOfVaultFolder duong dan den thu muc vualt (eg: .../hello/vault
 * @return  id cua bucket
 */
quint64 VaultController::ensureAvailableBucket(VaultDB &vDB, QString pathOfVaultFolder)
{
    vDB.bucket.setDb(&vDB._db_);
    /* --- 1. Kiem tra xem bucket nao phu hop --- */
    QVector<Bucket> availableBuckets = vDB.bucket.fetch("WHERE is_full = 0 LIMIT 1");
    if (!availableBuckets.isEmpty()) {
        return availableBuckets.first().id;
    }

    /* --- 2. Nếu không tìm thấy, tiến hành tạo mới --- */
    qint64  currentCount   = vDB.bucket.count(""); // SELECT COUNT(*) from bucket
    QString nameBucket     = QString("bucket_%1.txt").arg(currentCount + 1);
    QString fullPathBucket = pathOfVaultFolder + "/" + nameBucket;

    // Tao file vat li trong
    QFile file(fullPathBucket);
    if (!file.open(QIODevice::WriteOnly)) {
        LOG_ERROR("Cannot create physical file bucket at: " + fullPathBucket);
        return 0;
    }
    file.close();

    // Setup thong tin cho bucket new
    Bucket newBucket;
    newBucket.relative_path = fullPathBucket;
    newBucket.is_full       = 0;
    newBucket.slices        = QByteArray();
    if(!vDB.bucket.insert(newBucket)) {
        LOG_ERROR("Failed to insert new bucket record to DB");
        return 0;
    }

    // LOG_DEBUG("ID BUCKET: " + QString::number(newBucket.id));
    return newBucket.id;
}

/**
 * @brief Xử lí và đưa dữ liệu của file từ trong vault ra ngoài
 * @param fileId
 * @param destPath
 * @param fileName
 * @return
 */
bool VaultController::exportFileFromVault(int fileId, QString destPath, QString fileName) {

}
