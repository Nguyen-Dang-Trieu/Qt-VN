#include <openssl/rand.h>
#include "vaultcontroller.h"
#include "VaultDB.h"
#include "logger.h"
#include "support.h"
#include "aes_256_gcm.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>


VaultController::VaultController(QObject *parent)
    : QObject(parent)
{
    /*
     - Khởi tạo PathDB và gắn nó vào trong vaultcontroller.
     - Việc truyền con trỏ this vào giúp Path DB tự hủy khi VaultController bị hủy
    */
    pathDB = new CPathDB(this);

    // Note: CPathDB đã tự gọi createPathDB() trong constructor của nó nên DB sẽ tự động được mở ngay tại đây.
}

bool VaultController::openVault(const QString &vaultName, const QString &password, const QString &path) {
    // Test params
    LOG_DEBUG("[CONTROLLER] Vault Name: " + vaultName);
    LOG_DEBUG("[CONTROLLER] Password: " + password);
    LOG_DEBUG("[CONTROLLER] Path: " + path);

//    // 1. Lấy đường dẫn từ PathDB
//    QString vaultPath = pathDB->getVaultPathByName(vaultName);

//    if (vaultPath.isEmpty()) {
//        LOG_ERROR("Don't find " + vaultName);
//        return false;
//    }

    // 2. (Optional) Test mở VaultDB ở đây để check mật khẩu có đúng không ?

    // 3. Phát signal báo cho main.cpp (và NodeModel) biết đường dẫn & password
    emit vaultOpened(vaultName, password, path);
    return true;
}

bool VaultController::createVaultFolder(const QString &vaultName, const QString &password, const QString &path) {
    /* --- LOG DEBUG --- */
    // LOG_DEBUG("password DB: " + password);


    // ----------
    if (vaultName.isEmpty() || password.isEmpty() || path.isEmpty()) return false;

    QString basePath    = path + "/" + vaultName;
    QString dataDirPath = basePath + "/vault";
    QString dbPath      = basePath + "/" + vaultName + ".db";

    QDir dir;
    if (!dir.mkpath(dataDirPath)) {
        LOG_ERROR("[CONTROLLER] Don't create 'Data Dir' " + dataDirPath);
        return false;
    }

    /* Tạo folder vault */
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

    /* Tao folder vault thanh cong -> Luu thong tin va path vao trong Path DB*/
    bool saveToPathDB = pathDB->saveVaultFolder(vaultName, basePath);

    if (!saveToPathDB) {
        LOG_ERROR("Unable to save" + vaultName + "information into Path DB");
        return false;
    }

    /* ---------------------------------------- Them thong tin vault va masterkey vao trong Table Vault config --- */
    // Random ContentKey
    QByteArray ContentKey(32, 0); // 32 bytes
    ContentKey = randomKey();

    vDB.vaultConfig.setDb(&vDB._db_);
    VaultConfig infoVault; // Thong tin cua vault dang tao
    infoVault.contentKey = ContentKey.toHex();
    infoVault.version   = 1;
    vDB.vaultConfig.insert(infoVault);

    LOG_INFO("ContenKey: " + ContentKey.toHex());

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

// NEW 2024-14-4
QString VaultController::renameVault(const QString &oldPath, const QString &newNameVault) {
    QString cleanOldPath = oldPath;
    if (cleanOldPath.startsWith("file://")) {
        cleanOldPath = QUrl(oldPath).toLocalFile();
    }
    /* --- LOG DEBUG --- */
//    LOG_DEBUG("[CONTROLLER] old Path: " + cleanOldPath); //  /home/GhostRider/Work/clone-app-vault_/Test/output_test_app_1/q
//    LOG_DEBUG("[CONTROLLER] newName : " + newName);      // hello


    QDir dir(cleanOldPath);
    if (!dir.exists()) {
        LOG_ERROR("The " + dir.dirName() + " directory does not exist!");
        return "";
    }

    QFileInfo oldFileInfo(cleanOldPath);
    QString   oldVaultName  = oldFileInfo.fileName();
    QString pathParentDir   = oldFileInfo.absolutePath();    // /home/GhostRider/Work/clone-app-vault_/Test/output_test_app_1/
    QString   newPath       = pathParentDir + "/" + newNameVault;

    // Kiểm tra xem tên mới đã bị trùng với một thư mục khác chưa
    if (QDir(newPath).exists()) {
        LOG_ERROR("The " + newNameVault + " Vault already exists in folder: " + QFileInfo(pathParentDir).fileName());
        return "";
    }

    // .../output_test_app_1/q --->  .../output_test_app_1/hello
    LOG_DEBUG("Rename: " + QFileInfo(cleanOldPath).fileName() + " ---> " + QFileInfo(newPath).fileName());

    QString oldDbPath = cleanOldPath + "/" + oldVaultName + ".db";
    QString newDbPath = cleanOldPath + "/" + newNameVault + ".db";

    if (QFile::exists(oldDbPath)) {
        if (!QFile::rename(oldDbPath, newDbPath)) {
            LOG_ERROR("Failed to rename database file from " + oldVaultName + ".db to " + newNameVault + ".db");
            return "";
        }
    }

    /* --- Thực thi đổi tên trên ổ cứng --- */
    QDir rootDir;
    if (rootDir.rename(cleanOldPath, newPath)) {
        // Update Path DB
        if (pathDB->updateVaultFolder(cleanOldPath, newPath)) {
            LOG_INFO("[CONTROLLER] Rename vault and database file successful!");
            return newPath;
        } else {
            // Rollback nếu DB lỗi: Đổi tên thư mục và file DB lại như cũ
            rootDir.rename(newPath, cleanOldPath);
            QFile::rename(newPath + "/" + newNameVault + ".db", oldDbPath);
            LOG_ERROR("Database Error: Rollback folder and DB file name.");
        }
    } else {
        // Rollback file DB nếu đổi tên thư mục thất bại
        QFile::rename(newDbPath, oldDbPath);
        LOG_ERROR("The OS denies permission to rename the folder!");
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

    bool success = copyRecursively(cleanSource, destinationPath, vDB, rootId, basePath);

    if (!success) {
        LOG_INFO("Copy Failed:  " + srcInfo.fileName());
    }

    vDB.close();
    return success;
}

bool VaultController::copyRecursively(const QString &srcFilePath, const QString &targetFilePath, VaultDB &vDB, int parentId, const QString &basePath)
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
            if (!copyRecursively(newSrc, newTgt, vDB, newNode.m_id, basePath)) {
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

        ingestFileToBuckets(vDB, id_file, srcFilePath, basePath);
    }

    return true;
}

bool VaultController::ingestFileToBuckets(VaultDB &vDB, quint64 nodeID,
                                         QString pathSrcFile,
                                         QString pathOfVaultFolder)
{
    vDB.nodeData.setDb(&vDB._db_);
    vDB.bucket.setDb(&vDB._db_);
    vDB.vaultConfig.setDb(&vDB._db_);

    /* ---- Get Content Key from Table Vault_Config ---- */
    QVector<VaultConfig> configs = vDB.vaultConfig.fetch("LIMIT 1");
    if (configs.isEmpty()) {
        LOG_ERROR("Table vault config empty!");
        return false;
    }

    QByteArray contentKey = QByteArray::fromHex(configs.first().contentKey.toUtf8());
    if (contentKey.isEmpty()) {
        LOG_ERROR("Content Key empty!");
        return false;
    }

    /* ---- Tìm & Mở Bucket ---- */
    quint64 bucketID = ensureAvailableBucket(vDB, pathOfVaultFolder);
    if (bucketID == 0) {
        LOG_ERROR("It is not possible to obtain or create Bucket!");
        return false;
    }

    // Lay toan bo thong tin cua file bucket theo bucketID tim duoc
    QVector<Bucket> results = vDB.bucket.fetch(QString("WHERE id = '%1' LIMIT 1").arg(bucketID));
    if(results.isEmpty()) {
        LOG_ERROR("No bucket found with ID: " + QString::number(bucketID));
        return false;
    }

    Bucket currBucket = results.first();
    LOG_DEBUG("targetBucket id:" + QString::number(currBucket.id));

    QString fullBucketPath = pathOfVaultFolder + "/" + currBucket.relative_path;
    QFile bucketFile(fullBucketPath);
    if(!bucketFile.open(QIODevice::Append | QIODevice::ReadWrite)) {
        LOG_ERROR("Failed to open Bucket File! - Path bucket " + currBucket.relative_path);
        return false;
    }
    QJsonArray arrSlices = QJsonDocument::fromJson(currBucket.slices).array();

    /* --- Mở File Nguồn --- */
    QFile srcFile(pathSrcFile);
    if(!srcFile.open(QIODevice::ReadOnly)) {
        LOG_ERROR("Unable to open source file: " + QFileInfo(pathSrcFile).fileName());
        return false;
    }
    /* --- Đọc FIlE và mã hóa từng Chunk --- */
    quint64 slice_number = 0;
    QByteArray chunk;


    while (!(chunk = srcFile.read(CHUNK_SIZE)).isEmpty()) {
        // Kiem tra size cua bucket hien tai truoc khi ghi
        if(bucketFile.size() + SLICE_SIZE > BUCKET_SIZE) {
            // Luu trang thai bucket hien tai
            currBucket.is_full = 1;
            currBucket.slices = QJsonDocument(arrSlices).toJson(QJsonDocument::Compact);
            vDB.bucket.save(currBucket);
            bucketFile.close();
            LOG_DEBUG("Bucket " + QString::number(bucketID) + " full, creating new one...");

            // Lấy bucket mới
            bucketID = ensureAvailableBucket(vDB, pathOfVaultFolder);
            results = vDB.bucket.fetch(QString("WHERE id = '%1' LIMIT 1").arg(bucketID));
            if(results.isEmpty()) return false;

            currBucket = results.first();
            QString nextBucketPath = pathOfVaultFolder + "/" + currBucket.relative_path;
            bucketFile.setFileName(nextBucketPath);
            if(!bucketFile.open(QIODevice::Append | QIODevice::ReadWrite)) {
                LOG_ERROR("[CRITICAL] Unable to open a new bucket file at: " + nextBucketPath);
                return false;
            }

            // C. Reset mảng JSON cho bucket mới
            arrSlices = QJsonDocument::fromJson(currBucket.slices).array();
        }

        QString sliceUuid   = QFileInfo(pathSrcFile).completeBaseName() + "-" + generateRandomLetters();
        qint64 currentOffset = bucketFile.pos();

        // Padding cho Chunk cuối cùng nếu bị thiếu byte
        if(chunk.size() < CHUNK_SIZE) {
            chunk.append(CHUNK_SIZE - chunk.size(), (char)0x00);
        }

        // Params cho GCM
        QByteArray chunkNonce = randomHeaderNonce(); // 12 bytes ngẫu nhiên
        QByteArray chunkTag(16, 0);                  // 16 bytes Tag
        QByteArray encryptedChunk(CHUNK_SIZE, 0);    // 64 KiB
        QByteArray chunkAAD   = QByteArray::number(slice_number); // Dùng slice_number chống tráo đổi

        // Mã hóa nội dung file bằng ContentKey
        if(!aes_gcm_encrypt(chunk, contentKey, chunkNonce, chunkAAD, encryptedChunk, chunkTag)) {
            LOG_ERROR("Failed to encrypt chunk number: " + QString::number(slice_number));
            return false;
        }

//        QtConcurrent::run([this](){
//            // Example: Quit a thread safely from another thread
//            QMetaObject::invokeMethod(this, [](){

//            }, Qt::QueuedConnection);

//        });

        // Ghi vao bucket: Nonce (12 Bytes) + ciphertext chunk (64 KiB) + Tag ( 16 Bytes) = SLICE
        bucketFile.write(chunkNonce);
        bucketFile.write(encryptedChunk);
        bucketFile.write(chunkTag);

        // Update JSON cho bucket
        QJsonObject sliceObj;
        sliceObj["slice_uuid"] = sliceUuid;
        sliceObj["offset"]     = currentOffset;
        arrSlices.append(sliceObj);

        // Insert vào table node_data
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

    currBucket.slices = QJsonDocument(arrSlices).toJson(QJsonDocument::Compact);
    // Nếu bucket cuối cùng xấp xỉ 1MB,  đánh dấu is_full = 1
    if (bucketFile.size() + SLICE_SIZE > BUCKET_SIZE) {
        currBucket.is_full = 1;
    }
    vDB.bucket.save(currBucket);

    bucketFile.close();
    srcFile.close();

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

    for (const Bucket &b : availableBuckets) {
        QString fullPath = pathOfVaultFolder + "/" + b.relative_path;
        QFileInfo info(fullPath);

        if (info.exists() && info.size() + SLICE_SIZE < BUCKET_SIZE) {
            return b.id;
        } else { // File bucket is full
            Bucket fullB = b;
            fullB.is_full = 1;
            vDB.bucket.save(fullB);
        }
    }

    /* --- 2. Nếu không tìm thấy, tiến hành tạo mới --- */
    qint64  currentCount   = vDB.bucket.count(""); // SELECT COUNT(*) from bucket
    QString nameBucket     = QString("bucket_%1.bin").arg(currentCount + 1);
    QString relativePath   = "vault/" + nameBucket;
    QString fullPathBucket = pathOfVaultFolder + "/" + relativePath;

    // Tao file vat li trong
    QFile file(fullPathBucket);
    if (!file.open(QIODevice::WriteOnly)) {
        LOG_ERROR("Can't create physical file bucket at: " + fullPathBucket);
        return 0;
    }
    file.close();

    // Setup thong tin cho bucket new
    Bucket newBucket;
    newBucket.relative_path = relativePath;
    newBucket.is_full       = 0;
    newBucket.slices        = QByteArray("[]");
    if(!vDB.bucket.insert(newBucket)) {
        LOG_ERROR("Failed to insert new bucket record to DB");
        return 0;
    }

    // LOG_DEBUG("ID BUCKET: " + QString::number(newBucket.id));
    return newBucket.id;
}

/**
 * @brief Xử lí và đưa dữ liệu của file từ trong vault ra ngoài
 * @param vaultName  Tên của folder name mà user đang tương tác
 */
bool VaultController::exportFileFromVault(QString destPath, QString vaultName, QString vaultPass, int fileId, QString fileName) {
    /* ---- LOGIN DEBUG ---- */
    LOG_INFO("exportFileFromVault: Start exporting " + fileName);

    // -------------- Start
    // 1. Lấy thông tin đường dẫn Vault
    QString query = QString("WHERE name = '%1' LIMIT 1").arg(vaultName);
    QVector<CSavedVault> results = pathDB->User_InfoVaults.fetch(query);
    if(results.isEmpty()) return false;

    CSavedVault &User_InfoVaults = results.first();
    QString dbPathVault = User_InfoVaults.path + "/" + vaultName + ".db";

    // 2. Mở DB
    VaultDB vDB;
    if (!vDB.open(dbPathVault, vaultPass)) return false;

    vDB.nodeData.setDb(&vDB._db_);
    vDB.bucket.setDb(&vDB._db_);
    vDB.vaultConfig.setDb(&vDB._db_);
    vDB.node.setDb(&vDB._db_);         // Mở table Node để lấy kích thước file gốc

    /* --- Get Content Key --- */
    QVector<VaultConfig> configs = vDB.vaultConfig.fetch("LIMIT 1");
    if (configs.isEmpty()) {
        LOG_ERROR("Table vault config empty!");
        return false;
    }
    QByteArray ContentKey = QByteArray::fromHex(configs.first().contentKey.toUtf8());

    /* --- Get Info file --- */
    QVector<Node> fileNodes = vDB.node.fetch(QString("WHERE id = %1 LIMIT 1").arg(fileId));
    if (fileNodes.isEmpty()) {
        LOG_ERROR("Table Node empty!");
        return false;
    }
    quint64 originalFileSize = fileNodes.first().m_length; // Dùng để cắt bỏ Padding lúc ghi xong

    // 3. Lấy danh sách các mảnh (sắp xếp theo slice_number để nối đúng thứ tự)
    QVector<NodeData> nodeDatas = vDB.nodeData.fetch(QString("WHERE node_id = %1 ORDER BY slice_number ASC").arg(fileId));
    if(nodeDatas.isEmpty()) {
        LOG_ERROR(fileName + " no data found in DB!");
        return false;
    }

    // 4. Mở file đích để chuẩn bị ghi
//    if (!destPath.startsWith("/")) {
//        destPath = "/" + destPath;
//    }
    QString outFilePath = QDir(destPath).filePath(fileName);
    QFile destFile(outFilePath);
    if (!destFile.open(QIODevice::WriteOnly)) {
        LOG_ERROR("Cannot create destination file: " + destFile.fileName());
        return false;
    }

    /* --- DUYỆT TỪNG SLICCE DE GIAI MA DATA --- */
    for (const NodeData &curNodeData : nodeDatas) {
        // 1.Tim dung bucket chua slice nay
        QVector<Bucket> buckets = vDB.bucket.fetch(QString("WHERE id = %1 LIMIT 1").arg(curNodeData.bucket_id));
        if (buckets.isEmpty()) {
            LOG_ERROR("Không tìm thấy Bucket ID: " + QString::number(curNodeData.bucket_id));
            return false;
        }
        Bucket &bucket = buckets.first();

        // Mo file bucket
        QString fullBucketPath = User_InfoVaults.path + "/" + bucket.relative_path;
        QFile bucketFile(fullBucketPath);
        if (!bucketFile.open(QIODevice::ReadOnly)) {
            LOG_ERROR("Không thể mở file vật lý: " + bucket.relative_path);
            return false;
        }


        QJsonArray arrSlices = QJsonDocument::fromJson(bucket.slices).array();
        qint64 offset = -1;

        for (int i = 0; i < arrSlices.size(); ++i) {
            QJsonObject obj = arrSlices[i].toObject();
            if (obj["slice_uuid"].toString() == curNodeData.slice_uuid) {
                offset = obj["offset"].toVariant().toLongLong();
                break;
            }
        }

        if (offset != -1 && bucketFile.seek(offset)) {
            // Đọc đúng cấu trúc 1 Slice đã ghi: Nonce (12) + Data (65536) + Tag (16)
            QByteArray chunkNonce     = bucketFile.read(12);
            QByteArray encryptedChunk = bucketFile.read(CHUNK_SIZE);
            QByteArray chunkTag       = bucketFile.read(16);

            // AAD phải chính xác là slice_number
            QByteArray chunkAAD = QByteArray::number(curNodeData.slice_number);

            // Giải mã Chunk bằng ContentKey
            QByteArray decryptedChunk = aes_gcm_decrypt(encryptedChunk, chunkTag, ContentKey, chunkNonce, chunkAAD);

            // Xác thực Tag của GCM
            if (decryptedChunk.isEmpty()) {
                LOG_ERROR("Lỗi MAC GCM: Slice " + QString::number(curNodeData.slice_number) + " bị hỏng hoặc tráo đổi vị trí!");
                destFile.close();
                destFile.remove(); // Hủy toàn bộ file vì dữ liệu không đáng tin
                return false;
            }

            // Ghi dữ liệu đã giải mã vào file đích
            destFile.write(decryptedChunk);
        } else {
            LOG_ERROR("Slice UUID not found in Bucket JSON: " + curNodeData.slice_uuid);
        }

        bucketFile.close();
    }

    // 6. Cắt bỏ Padding (bytes 0x00 thừa ở chunk cuối cùng)
    destFile.resize(originalFileSize);
    destFile.close();
    vDB.close();

    LOG_INFO("Export file " + fileName + " successful! Khôi phục dung lượng gốc: " + QString::number(originalFileSize) + " bytes.");
    return true;
}

