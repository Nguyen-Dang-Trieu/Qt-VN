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
    LOG_DEBUG("Vault Name: " + vaultName);
    LOG_DEBUG("Password: " + password);
    LOG_DEBUG("Path: " + path);

//    // 1. Lấy đường dẫn từ PathDB
//    QString vaultPath = pathDB->getVaultPathByName(vaultName);

//    if (vaultPath.isEmpty()) {
//        LOG_ERROR("Don't find " + vaultName);
//        return false;
//    }

    // 2. (Optional) Test mở VaultDB ở đây để check mật khẩu có đúng không ?

    // 3. Phát signal báo cho main.cpp (và NodeModel) biết đường dẫn & password
    emit vaultOpened(path, password);
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
        ingestFileToBuckets(vDB, id_file, srcFilePath, vaultDataDir);
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

    /* ---- Get Master Key ---- */
    QVector<VaultConfig> configs = vDB.vaultConfig.fetch("LIMIT 1");
    if (configs.isEmpty()) {
        LOG_DEBUG("Table vault config empty!");
        return false;
    }

    // 1. Tạo File Header (Bảo vệ ContentKey bằng MasterKey)
    QByteArray contentKey = randomKey();
    QByteArray fileHeader = createFileHeader(configs.first().masterKey.toUtf8(), contentKey);
    if (fileHeader.isEmpty()) {
        LOG_ERROR("Create Header File is Faild!");
        return false;
    }

    /* ---- Tìm & Mở Bucket ---- */
    quint64 bucketID = ensureAvailableBucket(vDB, pathOfVaultFolder);
    if (bucketID == 0) {
        LOG_ERROR("It is not possible to obtain or create Bucket!");
        return false;
    }

    QString query = QString("WHERE id = '%1' LIMIT 1").arg(bucketID);
    QVector<Bucket> results = vDB.bucket.fetch(query);
    if(results.isEmpty()) {
        LOG_ERROR("No bucket found with ID: " + QString::number(bucketID));
        return false;
    }

    Bucket &targetBucket = results.first();
    LOG_DEBUG("targetBucket id:" + QString::number(targetBucket.id));

    QString pathOfBucket = targetBucket.relative_path;
    QFile bucketFile(pathOfBucket);
    if(!bucketFile.open(QIODevice::Append | QIODevice::ReadWrite)) {
        LOG_ERROR("Failed to open Bucket File! " + pathOfBucket);
        return false;
    }

    /* --- Mở File Nguồn --- */
    QFile     srcFile(pathSrcFile);
    QFileInfo srcFileInfo(pathSrcFile);
    if(!srcFile.open(QIODevice::ReadOnly)) {
        LOG_ERROR("Unable to open source file: " + srcFileInfo.fileName());
        return false;
    }

    /* --- Ghi Header vào Bucket --- */
    // Lấy offset hiện tại (cuối file bucket) trước khi ghi Header của file NÀY
    // qint64 fileStartOffset = bucketFile.pos();

    // GHI 60 BYTES HEADER VÀO TRƯỚC.
    // Các slice của file này sẽ tự động nối tiếp ngay sau byte thứ 60.
    bucketFile.write(fileHeader);

    /* --- Đọc và Mã hóa từng Chunk --- */
    quint64 slice_number = 0;
    QByteArray chunk;

    QJsonDocument doc = QJsonDocument::fromJson(targetBucket.slices);
    QJsonArray    arrSlices;
    if(doc.isArray()) {
        arrSlices = doc.array();
    }

    while (!(chunk = srcFile.read(CHUNK_SIZE)).isEmpty()) {
        QString sliceUuid   = srcFileInfo.completeBaseName() + "-" + generateRandomLetters();

        // Offset này đã tự động tịnh tiến sau khi ghi 60 byte Header
        qint64 currentOffset = bucketFile.pos();

        // Padding cho Chunk cuối cùng nếu bị thiếu byte
        if(chunk.size() < CHUNK_SIZE) {
            chunk.append(CHUNK_SIZE - chunk.size(), (char)0x00);
        }

        // Chuẩn bị biến cho GCM
        QByteArray chunkNonce = randomHeaderNonce(); // 12 bytes ngẫu nhiên
        QByteArray chunkTag(16, 0);                  // 16 bytes Tag
        QByteArray encryptedChunk(CHUNK_SIZE, 0);    // 64 KiB
        QByteArray chunkAAD = QByteArray::number(slice_number); // Dùng slice_number chống tráo đổi

        // Mã hóa nội dung file bằng ContentKey
        bool encryptOk = aes_gcm_encrypt(chunk, contentKey, chunkNonce, chunkAAD, encryptedChunk, chunkTag);
        if(!encryptOk) {
            LOG_ERROR("Failed to encrypt chunk number: " + QString::number(slice_number));
            return false;
        }

        // Ghi vào file bucket vật lý theo đúng chuẩn SLICE_SIZE = 12 + 65536 + 16 = 65564 bytes
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
        // Không cần curr.header nữa vì đã lưu ở file vật lý

        if (!vDB.nodeData.insert(curr)) {
            LOG_ERROR("Cannot add slice to node_data table!");
            return false;
        }

        slice_number++;
    }

    bucketFile.close();
    srcFile.close();

    // Lưu chuỗi JSON đã update vào table Bucket
    QJsonDocument newDoc(arrSlices);
    targetBucket.slices = newDoc.toJson(QJsonDocument::Compact);
    if(vDB.bucket.save(targetBucket)) {
        LOG_DEBUG("Update table bucket slices successful. Saved " + QString::number(slice_number) + " slices.");
    } else {
        LOG_ERROR("Failed to save slices to bucket table.");
    }

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
 * @param vaultName  Tên của folder name mà user đang tương tác
 */
bool VaultController::exportFileFromVault(QString destPath, QString vaultName, QString vaultPass, int fileId, QString fileName) {
    LOG_INFO("exportFileFromVault: Start exporting " + fileName);

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

    /* --- Lấy Master Key và Kích thước file gốc --- */
    QVector<VaultConfig> row_1 = vDB.vaultConfig.fetch("LIMIT 1");
    if (row_1.isEmpty()) return false;
    QByteArray rawMasterKey = row_1.first().masterKey.toUtf8();

    QVector<Node> fileNodes = vDB.node.fetch(QString("WHERE id = %1 LIMIT 1").arg(fileId));
    if (fileNodes.isEmpty()) return false;
    quint64 originalFileSize = fileNodes.first().m_length; // Dùng để cắt bỏ Padding lúc ghi xong

    // 3. Lấy danh sách các mảnh (sắp xếp theo slice_number để nối đúng thứ tự)
    query = QString("WHERE node_id = %1 ORDER BY slice_number ASC").arg(fileId);
    QVector<NodeData> nodeDatasOfFile = vDB.nodeData.fetch(query);

    if(nodeDatasOfFile.isEmpty()) {
        LOG_ERROR(fileName + " no data found in DB!");
        return false;
    }

    // 4. Mở file đích để chuẩn bị ghi
    /* Dành cho Ubuntu */
//    if (!destPath.startsWith("/")) {
//        destPath = "/" + destPath;
//    }

    /* Dành cho window */
    // Ghi chú: Không nên hardcode + ".txt" vì nếu lưu ảnh/pdf sẽ hỏng đuôi file
    QString outFilePath = QDir(destPath).filePath(fileName);
    // QDir::filePath sẽ tự động xử lý dấu gạch chéo giữa folder và tên file một cách chuẩn xác.

    QFile destFile(outFilePath);
    if (!destFile.open(QIODevice::WriteOnly)) {
        LOG_ERROR("Cannot create destination file: " + destFile.fileName());
        // In thêm lỗi của QFile để debug chính xác hơn
        LOG_ERROR("System Error: " + destFile.errorString());
        return false;
    }

    /* --- BƯỚC QUAN TRỌNG: PHỤC HỒI CONTENT KEY TỪ FILE HEADER --- */
    QByteArray ContentKey;
    {
        NodeData firstSlice = nodeDatasOfFile.first();
        QVector<Bucket> buckets = vDB.bucket.fetch(QString("WHERE id = %1 LIMIT 1").arg(firstSlice.bucket_id));
        if (buckets.isEmpty()) return false;

        QFile bucketFile(buckets.first().relative_path);
        if (!bucketFile.open(QIODevice::ReadOnly)) return false;

        QJsonDocument doc = QJsonDocument::fromJson(buckets.first().slices);
        QJsonArray arrSlices = doc.array();
        qint64 firstSliceOffset = -1;
        for (int i = 0; i < arrSlices.size(); ++i) {
            QJsonObject obj = arrSlices[i].toObject();
            if (obj["slice_uuid"].toString() == firstSlice.slice_uuid) {
                firstSliceOffset = obj["offset"].toVariant().toLongLong();
                break;
            }
        }

        if (firstSliceOffset == -1) return false;

        // Seek và đọc đúng 68 bytes Header
        if (!bucketFile.seek(firstSliceOffset - 68)) {
            LOG_ERROR("Lỗi: Không thể seek tới vị trí File Header!");
            return false;
        }
        QByteArray headerData = bucketFile.read(68);
        bucketFile.close();

        // --- ĐÂY LÀ CHỖ THAY ĐỔI DUY NHẤT ---
        ContentKey = decryptContentKey(headerData, rawMasterKey);

        if (ContentKey.isEmpty()) {
            LOG_ERROR("Header Decryption Error: Incorrect Master Key or Corrupted Data!");
            destFile.close();
            destFile.remove();
            return false;
        }
        // ------------------------------------

        LOG_DEBUG("ContentKey recovery successful! Preparing to decrypt the fragments...");
    }

    /* --- BƯỚC 5: DUYỆT TỪNG MẢNH ĐỂ GIẢI MÃ DATA --- */
    for (const NodeData &curNodeData : nodeDatasOfFile) {
        QString bQuery = QString("WHERE id = %1 LIMIT 1").arg(curNodeData.bucket_id);
        QVector<Bucket> buckets = vDB.bucket.fetch(bQuery);

        if (buckets.isEmpty()) continue;
        Bucket &bucket = buckets.first();

        QFile bucketFile(bucket.relative_path);
        if (!bucketFile.open(QIODevice::ReadOnly)) continue;

        QJsonDocument doc = QJsonDocument::fromJson(bucket.slices);
        QJsonArray arrSlices = doc.array();
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
            QByteArray chunkNonce   = bucketFile.read(12);
            QByteArray encryptedChunk = bucketFile.read(CHUNK_SIZE);
            QByteArray chunkTag     = bucketFile.read(16);

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

/**
 * @brief Tao File Header 68 Bytes (12 Bytes IV + 8 Bytes 0xFF + 32 Bytes Encrypted ContentKey + 16 Bytes Tag)
 * @return
 */
QByteArray VaultController::createFileHeader(const QByteArray &masterKey, const QByteArray &contentKey) {
    QByteArray headerNonce = randomHeaderNonce(); // 12 Bytes
    QByteArray cleartextPayload = QByteArray(8, (char)0xFF) + contentKey; // 40 bytes
    QByteArray ciphertextPayload(40, 0);
    QByteArray headerTag(16, 0);
    QByteArray headerAAD(12, 0);

    if (!aes_gcm_encrypt(cleartextPayload, masterKey, headerNonce, headerAAD, ciphertextPayload, headerTag)) {
        return QByteArray();
    }
    return headerNonce + ciphertextPayload + headerTag; // 68 bytes
}

/**
 * @brief Lay ContentKey tu Header File
 * @return
 */
QByteArray VaultController::decryptContentKey(const QByteArray &headerData, const QByteArray &masterKey) {
    if (headerData.size() < 68) return QByteArray();

    QByteArray nonce = headerData.left(12);
    QByteArray cipher = headerData.mid(12, 40);
    QByteArray tag = headerData.mid(52, 16);
    QByteArray aad(12, 0);

    QByteArray decrypted = aes_gcm_decrypt(cipher, tag, masterKey, nonce, aad);

    // Kiểm tra Magic Bytes 0xFF
    if (decrypted.isEmpty() || decrypted.left(8) != QByteArray(8, (char)0xFF)) {
        return QByteArray();
    }
    return decrypted.mid(8); // Trả về 32 bytes ContentKey
}
