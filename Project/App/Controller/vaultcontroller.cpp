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
    //    LOG_DEBUG("[CONTROLLER] Vault Name: " + vaultName);
    //    LOG_DEBUG("[CONTROLLER] Password: " + password);
    //    LOG_DEBUG("[CONTROLLER] Path: " + path);

    //    // 1. Lấy đường dẫn từ PathDB
    //    QString vaultPath = pathDB->getVaultPathByName(vaultName);

    //    if (vaultPath.isEmpty()) {
    //        LOG_ERROR("Don't find " + vaultName);
    //        return false;
    //    }

    // 2. (Optional) Test mở VaultDB ở đây để check mật khẩu có đúng không ?

    // 3. Phát signal báo cho main.cpp (và NodeModel) biết đường dẫn & password
    // QString nameDB = pathDB->getNameDB(vaultName);
    emit vaultOpened(vaultName, password, path);
    return true;
}

bool VaultController::createVaultFolder(const QString &vaultName, const QString &password, const QString &path) {
    /* --- LOG DEBUG --- */
    // LOG_DEBUG("password DB: " + password);


    // ----------
    if (vaultName.isEmpty() || password.isEmpty() || path.isEmpty()) return false;
    QString randomDbName = QUuid::createUuid().toString(QUuid::WithoutBraces).toLower();

    QString basePath    = path + "/" + vaultName;
    QString dataDirPath = basePath + "/vault";
    QString dbPath      = basePath + "/" + randomDbName + ".db";

    QDir dir;
    if (!dir.mkpath(dataDirPath)) {
        LOG_ERROR("[CONTROLLER] Don't create 'Data Dir' " + dataDirPath);
        return false;
    }

    /* Tạo folder vault */
    VaultDB vDB;
    bool isDbReady = vDB.open(dbPath, password);
    if (isDbReady) {
        // LOG_INFO("Vault DB created successfully at: " + randomDbName);
        pathDB->saveVaultFolder(vaultName, randomDbName, basePath);
    } else {
        LOG_ERROR("An Error ocurred when init db for folder vault!");
        return false;
    }

    if (!vDB.migrate()) {
        LOG_ERROR("Migrate Database Failed!");
        return false;
    }

    /* Tao folder vault thanh cong -> Luu thong tin va path vao trong Path DB*/
    bool saveToPathDB = pathDB->saveVaultFolder(vaultName, randomDbName, basePath);

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

// NEW 2024-15-4
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

// New add: 2026-04-22-drew
bool VaultController::importDataToVaultAsync(const QStringList &sourcePaths, const QString &vaultName, const QString &password) {
    if (sourcePaths.isEmpty()) return false;

    QString basePath = pathDB->getVaultPathByName(vaultName);
    QString nameDB   = pathDB->getNameDB(vaultName);
    if(basePath.isEmpty()) return false;

    QtConcurrent::run([=]() {
        QString dbPath = basePath + "/" + nameDB + ".db";
        QString destinationDir = basePath + "/vault/";

        VaultDB workerDB;
        if (!workerDB.open(dbPath, password)) {
            QMetaObject::invokeMethod(this, [=]() { emit importFinished(false); }, Qt::QueuedConnection);
            return;
        }
        workerDB.node.setDb(&workerDB._db_);

        // Lấy Root ID một lần
        QVector<Node> roots = workerDB.node.query("select * from node WHERE name = 'Root' AND type = 1 LIMIT 1");
        if (roots.isEmpty()) {
            workerDB.close();
            QMetaObject::invokeMethod(this, [=]() { emit importFinished(false); }, Qt::QueuedConnection);
            return;
        }
        int rootId = roots[0].m_id;

        bool allSuccess = true;

        // Vòng lặp xử lý từng file nằm TRONG thread ngầm
        for (const QString &path : sourcePaths) {
            QString cleanSource = path;
            if (cleanSource.startsWith("file://")) {
                cleanSource = QUrl(path).toLocalFile();
            }

            QFileInfo srcInfo(cleanSource);
            QString destinationPath = destinationDir + srcInfo.fileName();

            // Gọi hàm copy (hàm này đã có sẵn logic xử lý file/folder)
            if (!copyRecursively(cleanSource, destinationPath, workerDB, rootId, basePath)) {
                allSuccess = false;
                // Có thể break hoặc tiếp tục tùy bạn
            }
        }

        workerDB.close();

        // Chỉ phát tín hiệu xong khi ĐÃ XỬ LÝ HẾT danh sách
        QMetaObject::invokeMethod(this, [=]() {
            emit importFinished(allSuccess);
        }, Qt::QueuedConnection);
    });

    return true;
}

// NEW 2024-15-4
bool VaultController::copyRecursively(const QString &srcFilePath, const QString &targetFilePath, VaultDB &vDB, int parentId, const QString &basePath)
{
    QFileInfo srcFileInfo(srcFilePath);

    // Chuẩn bị object Node để chuẩn bị insert vào DB
    Node newNode;
    newNode.m_name = srcFileInfo.fileName();
    newNode.m_parent = parentId;
    //newNode.m_time = QDateTime::currentSecsSinceEpoch();
    // add start 20260421
    newNode.m_time = srcFileInfo.lastModified().toSecsSinceEpoch();
    newNode.m_atime = srcFileInfo.lastRead().toSecsSinceEpoch();
    // add end 20260421
    // --- TRƯỜNG HỢP LÀ THƯ MỤC ---
    if (srcFileInfo.isDir()) {
        //        QDir targetDir(targetFilePath);
        //        targetDir.cdUp();
        //        if (!targetDir.mkdir(srcFileInfo.fileName())) {
        //        }

        // 2. Insert Folder vào Database
        newNode.m_type = 1; // 1: Folder
        if (!vDB.node.insert(newNode)) {
            LOG_ERROR("DB Error: Không thể chèn Folder " + newNode.m_name);
            return false;
        }

        // 3. Duyệt các file con bên trong
        QDir sourceDir(srcFilePath);
        QStringList listFile = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

        if (listFile.isEmpty()) {
            // Danh sách đang trống
            LOG_DEBUG("List is empty!");
        }

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
        //        if (!QFile::copy(srcFilePath, targetFilePath)) {
        //            LOG_ERROR("Disk Error: Không thể copy file tới " + targetFilePath);
        //            return false;
        //        }

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
// NEW 2024-15-4
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
    if(!bucketFile.open(QIODevice::ReadWrite)) {
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
    quint64    slice_number = 0;
    QByteArray chunk;


    while (!(chunk = srcFile.read(CHUNK_SIZE)).isEmpty()) {
        qint64 currentOffset = findAvailableOffset(arrSlices);

        // Kiem tra size cua bucket hien tai truoc khi ghi
        if(currentOffset == -1) {
            // Luu trang thai bucket hien tai
            currBucket.is_full = 1;
            currBucket.slices = QJsonDocument(arrSlices).toJson(QJsonDocument::Compact);
            vDB.bucket.save(currBucket);
            bucketFile.close();

            LOG_DEBUG("Bucket " + QString::number(bucketID) + " full, creating new one...");

            // Tim hoac tao bucket moi
            bucketID = ensureAvailableBucket(vDB, pathOfVaultFolder);
            results = vDB.bucket.fetch(QString("WHERE id = '%1' LIMIT 1").arg(bucketID));
            if(results.isEmpty()) {
                LOG_ERROR("Bucket with ID " + QString::number(bucketID) + " not found!");
                return false;
            }
            currBucket = results.first();

            QString nextBucketPath = pathOfVaultFolder + "/" + currBucket.relative_path;
            bucketFile.setFileName(nextBucketPath);
            if(!bucketFile.open(QIODevice::ReadWrite)) {
                LOG_ERROR("[CRITICAL] Unable to open a new bucket file at: " + nextBucketPath);
                return false;
            }

            // C. Reset mảng JSON cho bucket mới
            arrSlices = QJsonDocument::fromJson(currBucket.slices).array();
            currentOffset = findAvailableOffset(arrSlices);
        }

        QString sliceUuid    = QFileInfo(pathSrcFile).completeBaseName() + "-" + generateRandomLetters();

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

        //        Tao thread moi (TODO)
        //        QtConcurrent::run([this](){
        //            // Example: Quit a thread safely from another thread
        //            QMetaObject::invokeMethod(this, [](){

        //            }, Qt::QueuedConnection);

        //        });

        // Ghi vao bucket: Nonce (12 Bytes) + ciphertext chunk (64 KiB) + Tag ( 16 Bytes) = SLICE
        bucketFile.seek(currentOffset);
        bucketFile.write(chunkNonce);
        bucketFile.write(encryptedChunk);
        bucketFile.write(chunkTag);

        // Update JSON cho bucket
        bool isReused = false;
        for (int i = 0; i < arrSlices.size(); ++i) {
            QJsonObject obj = arrSlices[i].toObject();
            if (obj["offset"].toVariant().toLongLong() == currentOffset) {
                // Ghi đè UUID mới vào vị trí cũ
                obj["slice_uuid"] = sliceUuid;
                arrSlices[i] = obj;
                isReused = true;
                break;
            }
        }

        // Nếu là offset hoàn toàn mới (chưa từng ghi), thì mới append
        if (!isReused) {
            QJsonObject sliceObj;
            sliceObj["slice_uuid"] = sliceUuid;
            sliceObj["offset"]     = currentOffset;
            arrSlices.append(sliceObj);
        }
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

    // Kiểm tra lại lần cuối xem bucket đã thực sự full chưa
    if (findAvailableOffset(arrSlices) == -1) {
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
    QVector<Bucket> availableBuckets = vDB.bucket.fetch("WHERE is_full = 0");

    for (const Bucket &b : availableBuckets) {
        QJsonArray arrSlices = QJsonDocument::fromJson(b.slices).array();

        // Kiểm tra 1: Còn chỗ trống để append mới không?
        bool hasSpaceToAppend = (arrSlices.size() < SLICES_PER_BUCKET);

        // Kiểm tra 2: Có vị trí nào bị "NULL" để ghi đè không?
        bool hasDeletedSlots = false;
        for (const QJsonValue &val : arrSlices) {
            QString uuid = val.toObject()["slice_uuid"].toString();
            if (uuid == "NULL" || uuid.isEmpty()) {
                hasDeletedSlots = true;
                break;
            }
        }
        if (hasSpaceToAppend || hasDeletedSlots) {
            return b.id; // Tái sử dụng bucket này
        } else {
            // Nếu không còn slot trống và cũng không có NULL -> Thực sự Full
            Bucket fullB = b;
            fullB.is_full = 1;
            vDB.bucket.save(fullB);
        }
    }

    /* --- 2. Nếu không tìm thấy, tiến hành tạo mới --- */
    QString uuid           = QUuid::createUuid().toString(QUuid::WithoutBraces).toLower();
    QString nameBucket     = uuid + ".bin";
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

// NEW 2026-04-17
/**
 * @brief Xử lí và đưa dữ liệu của file từ trong vault ra ngoài
 * @param vaultName  Tên của folder name mà user đang tương tác
 */
bool VaultController::exportFileFromVault(QString destPath, QString vaultName, QString vaultPass, int fileId, QString fileName, int fileType) {
    /* ---- LOGIN DEBUG ---- */
//    LOG_INFO("[CONTROLLER] Start exporting!");
//    LOG_DEBUG("[CONTROLLER] Name: "      + fileName);
//    LOG_DEBUG("[CONTROLLER] Type: "      + QString::number(fileType));
//    LOG_DEBUG("[CONTROLLER] dest Path: " + destPath);

    // -------------- Start
    // 1. Lấy thông tin đường dẫn Vault
    QString query = QString("WHERE name = '%1' LIMIT 1").arg(vaultName);
    QVector<CSavedVault> results = pathDB->User_InfoVaults.fetch(query);
    if(results.isEmpty()) return false;

    CSavedVault &User_InfoVaults = results.first();
    QString nameDB = pathDB->getNameDB(vaultName);

    QString dbPathVault = User_InfoVaults.path + "/" + nameDB + ".db";

    /* --- Mở DB --- */
    VaultDB vDB;
    if (!vDB.open(dbPathVault, vaultPass)) return false;

    vDB.nodeData.setDb(&vDB._db_);
    vDB.bucket.setDb(&vDB._db_);
    vDB.vaultConfig.setDb(&vDB._db_);
    vDB.node.setDb(&vDB._db_);

    /* --- Get Content Key --- */
    QVector<VaultConfig> configs = vDB.vaultConfig.fetch("LIMIT 1");
    if (configs.isEmpty()) {
        LOG_ERROR("Table vault config empty!");
        vDB.close();
        return false;
    }
    QByteArray ContentKey = QByteArray::fromHex(configs.first().contentKey.toUtf8());


    bool isSuccess = exportNodeRecursive(vDB, destPath, fileId, fileName, fileType, ContentKey, User_InfoVaults.path);

    vDB.close();
    return isSuccess;
}

// NEW 2026-04-17
bool VaultController::exportNodeRecursive(VaultDB &vDB, QString destPath, int nodeId, QString nodeName, int nodeType, const QByteArray &contentKey,
                                          const QString &vaultPath) {
    if (!destPath.startsWith("/")) {
        destPath = "/" + destPath;
    }

    if (nodeType == 1) { // FOLDER
        // Tạo thư mục mới
        QString newFolderPath = destPath + "/" + nodeName;
        QDir dir;
        if (!dir.mkpath(newFolderPath)) {
            LOG_ERROR("Unable to create folder: " + newFolderPath);
            return false;
        }

        // Truy vấn tất cả các node con nằm trong thư mục này
        QVector<Node> childNodes = vDB.node.fetch(QString("WHERE parent_id = %1").arg(nodeId));
        if (childNodes.isEmpty()) {
            LOG_INFO("Folder empty: " + nodeName);
            return true;
        }

        bool allSuccess = true;
        for (const auto &childNode : childNodes) {
            bool success = exportNodeRecursive(vDB, newFolderPath, childNode.m_id, childNode.m_name, childNode.m_type, contentKey, vaultPath);
            if (!success) {
                allSuccess = false;
            }
        }
        return allSuccess;

    } else { // FILE
        // Gọi hàm xử lý giải mã 1 file
        return decryptFile(vDB, destPath, nodeId, nodeName, contentKey, vaultPath);
    }
}

// NEW 2026-04-17
bool VaultController::decryptFile(VaultDB &vDB, QString destPath, int fileId, QString fileName, const QByteArray &contentKey, const QString &vaultPath) {
    /* --- Get Info file --- */
    QVector<Node> fileNodes = vDB.node.fetch(QString("WHERE id = %1 LIMIT 1").arg(fileId));
    if (fileNodes.isEmpty()) {
        LOG_ERROR("Không tìm thấy Node ID: " + QString::number(fileId));
        return false;
    }
    quint64 originalFileSize = fileNodes.first().m_length;

    // Lấy danh sách các mảnh (sắp xếp theo slice_number)
    QVector<NodeData> nodeDatas = vDB.nodeData.fetch(QString("WHERE node_id = %1 ORDER BY slice_number ASC").arg(fileId));
    if(nodeDatas.isEmpty() && originalFileSize > 0) {
        LOG_ERROR(fileName + " no data found in DB!");
        return false;
    }

    // Mở file đích để chuẩn bị ghi
    QString outFilePath = QDir(destPath).filePath(fileName);
    QFile destFile(outFilePath);
    if (!destFile.open(QIODevice::WriteOnly)) {
        LOG_ERROR("Cannot create destination file: " + destFile.fileName());
        return false;
    }

    /* --- DUYỆT TỪNG SLICE ĐỂ GIẢI MÃ --- */
    for (const NodeData &curNodeData : nodeDatas) {
        // Tim dung bucket chua slice nay
        QVector<Bucket> buckets = vDB.bucket.fetch(QString("WHERE id = %1 LIMIT 1").arg(curNodeData.bucket_id));
        if (buckets.isEmpty()) {
            LOG_ERROR("Không tìm thấy Bucket ID: " + QString::number(curNodeData.bucket_id));
            destFile.close();
            return false;
        }
        Bucket &bucket = buckets.first();

        // Mo file bucket
        QString fullBucketPath = vaultPath + "/" + bucket.relative_path;
        QFile bucketFile(fullBucketPath);
        if (!bucketFile.open(QIODevice::ReadOnly)) {
            LOG_ERROR("Không thể mở file vật lý: " + bucket.relative_path);
            destFile.close();
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
            QByteArray chunkNonce     = bucketFile.read(12);
            QByteArray encryptedChunk = bucketFile.read(CHUNK_SIZE);
            QByteArray chunkTag       = bucketFile.read(16);

            // AAD
            QByteArray chunkAAD = QByteArray::number(curNodeData.slice_number);

            // Giải mã Chunk
            QByteArray decryptedChunk = aes_gcm_decrypt(encryptedChunk, chunkTag, contentKey, chunkNonce, chunkAAD);

            // Xác thực Tag của GCM
            if (decryptedChunk.isEmpty()) {
                LOG_ERROR("Lỗi MAC GCM: Slice " + QString::number(curNodeData.slice_number) + " của " + fileName + " bị hỏng!");
                destFile.close();
                destFile.remove();
                return false;
            }

            destFile.write(decryptedChunk);
        } else {
            LOG_ERROR("Slice UUID not found in Bucket JSON: " + curNodeData.slice_uuid);
        }

        bucketFile.close();
    }

    // Cắt bỏ Padding (bytes 0x00 thừa ở chunk cuối cùng)
    destFile.resize(originalFileSize);
    destFile.close();

    LOG_INFO("Export file " + fileName + " successful! Size: " + QString::number(originalFileSize) + " bytes.");
    return true;
}


qint64 VaultController::findAvailableOffset(const QJsonArray &arrSlices) {
    // Ưu tiên tìm các slot đã bị xóa (có UUID là "NULL")
    for (int i = 0; i < arrSlices.size(); ++i) {
        QJsonObject obj = arrSlices[i].toObject();
        QString uuid = obj["slice_uuid"].toString();

        if (uuid == "NULL" || uuid.isEmpty()) {
            return obj["offset"].toVariant().toLongLong();
        }
    }

    // Nếu không có slot NULL, kiểm tra xem còn chỗ để append không (giới hạn 31 slices cho 1MB)
    if (arrSlices.size() < SLICES_PER_BUCKET) {
        // Offset mới = số lượng entry hiện tại * kích thước 1 slice
        return (qint64)arrSlices.size() * SLICE_SIZE;
    }

    // Bucket thực sự không còn chỗ trống nào
    return -1;
}

// NEW 2026-04-20
bool VaultController::deleteFile(const QString &nameVault, const QString &passVault, const QString &fileName, const QString &fileID) {
    /* LOG DEBUG */
    LOG_WARNING("deleteFile RUN!");
    LOG_DEBUG("[CONTROLLER] nameVault: " + nameVault);
    LOG_DEBUG("[CONTROLLER] passVault: " + passVault);
    LOG_DEBUG("[CONTROLLER] fileName:" + fileName);
    LOG_DEBUG("[CONTROLLER] fileID:" + fileID);

    /* Lay du lieu tu trong Path DB */
    QString basePathVault = pathDB->getVaultPathByName(nameVault); // ..../vaultName
    QString nameDB        = pathDB->getNameDB(nameVault);
    QString pathDBOfVault = basePathVault + "/" + nameDB + ".db";
    LOG_DEBUG("[CONTROLLER] basePathVault: " + basePathVault);

    /* Lay du lieu trong Vault DB */
    VaultDB vDB;
    if (!vDB.open(pathDBOfVault, passVault)) {
        LOG_ERROR("Unable to open Vault DB!");
        return false;
    }

    vDB.node.setDb(&vDB._db_);
    vDB.nodeData.setDb(&vDB._db_);
    vDB.delSlice.setDb(&vDB._db_);
    vDB.delNode.setDb(&vDB._db_);

    /* --- Delete --- */
    // Xoa trong table node_data
    QVector<NodeData> nodeDatas = vDB.nodeData.fetch(QString("WHERE node_id = %1").arg(fileID));
    if(nodeDatas.empty()) {
        LOG_ERROR("Table node data empty with node_id = " + fileID);
    }
    QMap<quint64, QStringList> slicesToFreePerBucket; // Nhóm các uuid cần xóa theo bucket_id

    // THem vao delslices
    for (const NodeData &curNodeData : nodeDatas) {
        LOG_DEBUG(curNodeData.slice_uuid);

        DelSlices slice;
        slice.m_sliceUuid = curNodeData.slice_uuid;
        slice.m_bucketId  = curNodeData.bucket_id;

        if (!vDB.delSlice.insert(slice)) {
            LOG_ERROR("Add slice to table delSlice failed!");
            return false;
        }

        // Luu slice_uuid de update JSON
        slicesToFreePerBucket[curNodeData.bucket_id].append(curNodeData.slice_uuid);
    }

    vDB.bucket.setDb(&vDB._db_);
    for (auto it = slicesToFreePerBucket.begin(); it != slicesToFreePerBucket.end(); ++it) {
        quint64 bucketID = it.key();
        const QStringList &uuidsToRemove = it.value();

        QVector<Bucket> buckets = vDB.bucket.fetch(QString("WHERE id = %1 LIMIT 1").arg(bucketID));
        if (!buckets.isEmpty()) {
            Bucket currBucket = buckets.first();
            QJsonArray arrSlices = QJsonDocument::fromJson(currBucket.slices).array();
            bool bucketModified = false;

            // Đổi uuid thành "NULL" trong JSON
            for (int i = 0; i < arrSlices.size(); ++i) {
                QJsonObject obj = arrSlices[i].toObject();
                if (uuidsToRemove.contains(obj["slice_uuid"].toString())) {
                    obj["slice_uuid"] = "NULL";
                    arrSlices[i] = obj; // Ghi đè object đã sửa vào array
                    bucketModified = true;
                }
            }

            if (bucketModified) {
                currBucket.slices = QJsonDocument(arrSlices).toJson(QJsonDocument::Compact);
                currBucket.is_full = 0; // ĐÁNH DẤU BUCKET NÀY KHÔNG CÒN ĐẦY NỮA
                vDB.bucket.save(currBucket);
                LOG_INFO("Đã giải phóng không gian cho Bucket ID: " + QString::number(bucketID));
            }
        }
    }

    // Xoa toan bo du lieu theo node_id chua trong table node_data sau khi them vao trong table delslice
    if (!vDB.nodeData.execQuery(QString("DELETE FROM %1 WHERE node_id = %2").arg(NodeData().tableName(), fileID))) {
        LOG_ERROR("Can't delete slices in table node data!");
        return false;
    }

    // Xoa node
    // 1. them vao delnode
    QVector<Node> nodes = vDB.node.fetch(QString("WHERE id = %1").arg(fileID));
    if(nodes.empty()) {
        LOG_ERROR("Table node data empty with node_id = " + fileID);
    }

    Node curNode = nodes.first();
    DelNode curNodeDel;
    curNodeDel.m_inode = curNode.m_id;
    if (!vDB.delNode.insert(curNodeDel)) {
        LOG_ERROR("Add node to table delNode failed!");
        return false;
    }

    // 2. Xoa node trong table node
    if (!vDB.node.execQuery(QString("DELETE FROM %1 WHERE id = '%2'").arg(Node().tableName(), fileID))) {
        LOG_ERROR("Can't delete node in table node!");
        return false;
    }

    return true;
}

bool VaultController::checkVaultStructure(const QString &folderPath) {
    QDir dir(folderPath);
    if (!dir.exists()) return false;

    QString vaultName = dir.dirName();

    // 1. Lấy danh sách tất cả (file + folder) trong thư mục Root
    QStringList rootEntries = dir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);

    int dbCount = 0;
    int vaultFolderCount = 0;

    // 2. KIỂM TRA ROOT: Chỉ cho phép đúng file .db và folder 'vault'
    for (const QString &entry : rootEntries) {
        if (entry == "vault") {
            vaultFolderCount++;
        }
        else if (entry == (vaultName + ".db")) {
            dbCount++;
        }
        else {
            // Nếu lọt vào đây tức là có một file hoặc folder LẠ nằm ở Root
            LOG_ERROR("Import Check: Phát hiện tệp/thư mục rác ở Root -> " + entry);
            return false;
        }
    }

    // 3. Đảm bảo có đủ mặt cả 2 thành phần (không thừa, không thiếu)
    if (dbCount != 1 || vaultFolderCount != 1) {
        LOG_ERROR("Import Check: Root bắt buộc phải có đúng 1 file .db và 1 thư mục 'vault'.");
        return false;
    }

    // 4. KIỂM TRA ĐỊNH DẠNG: Đảm bảo 'vault' là Folder và '.db' là File
    QFileInfo dbInfo(folderPath + "/" + vaultName + ".db");
    QFileInfo vaultFolderInfo(folderPath + "/vault");
    if (!dbInfo.isFile() || !vaultFolderInfo.isDir()) {
        LOG_ERROR("Import Check: Sai định dạng (vault phải là thư mục, .db phải là file).");
        return false;
    }

    // 5. KIỂM TRA BÊN TRONG THƯ MỤC 'vault'
    QDir vaultSubDir(folderPath + "/vault");
    QStringList subEntries = vaultSubDir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);

    for (const QString &entry : subEntries) {
        QFileInfo entryInfo(folderPath + "/vault/" + entry);

        // Không cho phép có thư mục con
        if (entryInfo.isDir()) {
            LOG_ERROR("Import Check: Phát hiện thư mục con nằm trong 'vault' -> " + entry);
            return false;
        }
        // Chỉ chấp nhận file bucket đuôi .txt
        if (!entry.endsWith(".bin")) {
            LOG_ERROR("Import Check: Phát hiện file rác không hợp lệ trong 'vault' -> " + entry);
            return false;
        }
    }

    LOG_INFO("Import Check: Cấu trúc Vault hoàn toàn hợp lệ và sạch sẽ!");
    return true;
}

// Add new: 2026-04-22-drew
QString VaultController::changeVaultPassword(const QString &vaultName, const QString &vaultPath, const QString &currentPass, const QString &newPass) {
    LOG_INFO("Request to change password for vault: " + vaultName);

    QString dbName = pathDB->getNameDB(vaultName);
    QString dbPath = vaultPath + "/" + dbName + ".db";
    VaultDB vDB;

    // Bước 1: Mở DB bằng mật khẩu CŨ để xác thực
    if (!vDB.open(dbPath, currentPass)) {
        LOG_ERROR("Sai mật khẩu hiện tại, không thể mở DB để đổi mật khẩu.");
        return ""; // Trả về rỗng nếu thất bại
    }

    // Bước 2: Đổi sang mật khẩu mới và lấy KEK mới
    QString newKek = "";
    if (vDB.changeVaultPassword(newPass)) {
        LOG_INFO("Đổi mật khẩu thành công, đang tạo KEK mới...");
        newKek = QString::fromUtf8(vDB.getVaultKEK()); // Lấy KEK mới
    }

    vDB.close();
    return newKek; // Trả về KEK
}
// add end 20260417

// Add new: 2026-04-22-drew
bool VaultController::verifyVaultKEK(const QString &vaultName, const QString &vaultPath, const QString &kekHex) {
    LOG_INFO("Đang xác thực KEK cho Vault: " + vaultName);

    QString dbName = pathDB->getNameDB(vaultName);
    QString dbPath = vaultPath + "/" + dbName + ".db";
    VaultDB vDB;

    bool isValid = vDB.openWithKEK(dbPath, kekHex);
    vDB.close();

    if (isValid) LOG_INFO("Xác thực KEK THÀNH CÔNG!");
    else LOG_ERROR("Xác thực KEK THẤT BẠI!");

    return isValid;
}

// Add new: 2026-04-22-drew
QJsonObject VaultController::resetVaultPasswordWithKEK(const QString &vaultName, const QString &vaultPath, const QString &kekHex, const QString &newPass) {
    LOG_INFO("Đang yêu cầu reset mật khẩu cho Vault: " + vaultName);

    QJsonObject result;
    result["success"] = false;
    result["newKek"] = "";

    // ---------
    QString dbName = pathDB->getNameDB(vaultName);
    QString dbPath = vaultPath + "/" + dbName + ".db";
    VaultDB vDB;

    // Bước 1: Dùng KEK để mở khóa DB thay vì dùng mật khẩu
    if (!vDB.openWithKEK(dbPath, kekHex)) {
        LOG_ERROR("KEK không hợp lệ, không thể mở DB để khôi phục mật khẩu.");
        return result; // Trả về false
    }

    // Bước 2: Tận dụng lại hàm changeVaultPassword có sẵn để đổi mật khẩu (và tự động tạo KEK mới)
    if (vDB.changeVaultPassword(newPass)) {
        LOG_INFO("Reset mật khẩu thành công!");
        result["success"] = true;
        // Trả về mã KEK MỚI (sau khi đổi pass) để giao diện QML hiển thị cho người dùng lưu lại
        result["newKek"] = QString::fromUtf8(vDB.getVaultKEK());
    } else {
        LOG_ERROR("Lỗi trong quá trình Rekey cơ sở dữ liệu.");
    }

    vDB.close();
    return result;
}
