#include "VaultDB.h"
#include "logger.h"

#define VAULT_APP_DB_VERSION 1

VaultDB::VaultDB() {}

VaultDB::~VaultDB() {}

/**
 * @brief Tạo mới DB or mở DB nếu đã có
 * @param dbPath
 * @param dbPassWord
 * @return
 */
bool VaultDB::open(const QString& dbPath, const QString& userPassword) {
    currentDBPath = dbPath;

    EzSql::DataBase::OpenParams params;
    params.dbPath = dbPath;
    params.flags  = SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE | SQLITE_OPEN_FULLMUTEX;
    params.tryKeys = QStringList() << "dummy_key"; // Key empty

//    if (!QFile::exists(dbPath)) { // Check File DB
//        // If don't have a DB
//        // 1. Generate with a random temporary Key
//        QString tempKey = QUuid::createUuid().toString();
//        params.tryKeys =  QStringList() << tempKey;

//        if (!_db_.open(params)) return false;

//        // 2. Create Table Note to create Salt
//        _db_.createTable<VaultConfig>();

//        // qDebug() << dbPath;
//        // qDebug() << userPassword;
//        QString currentKeyDB = deriveDbKey(dbPath, userPassword);
//        if(currentKeyDB.trimmed().isEmpty()) {
//            LOG_ERROR("currentKeyDB Empty!");
//            return false;
//        }

//        LOG_WARNING("Successfully currentKeyDB: " + currentKeyDB);

//        QString rekeyQuery = QString("PRAGMA rekey = \"x'%1'\";").arg(currentKeyDB);

//        char *zErrMsg = 0;
//        int rc = sqlite3_exec(_db_.connection(), rekeyQuery.toUtf8().data(), NULL, NULL, &zErrMsg);
//        _db_.close();

//        if(rc != SQLITE_OK) {
//            LOG_ERROR(QString("Rekey Error: %1").arg(zErrMsg));
//            sqlite3_free(zErrMsg);
//            return false;
//        }

//        LOG_INFO("Init and Rekey successful!");
//    }

//    QString realKey = deriveDbKey(dbPath, userPassword);
//    QString formattedKey = QString("x'%1'").arg(realKey);
//    params.tryKeys = QStringList() << formattedKey;

    // ------------------------------------- Thêm chỗ này hiện tại để test

    LOG_INFO("DB duoc tao voi dbPaht" + dbPath);

    if (_db_.open(params)) {
        _db_.createTable<VaultConfig>();
        return true;
    } else {
        LOG_ERROR("Open DB Failed!");
        return false;
    }
}

void VaultDB::close() { _db_.close(); }

/**
 * @brief Dẫn xuất Key
 * @param dbPath
 * @param userPassword
 * @return
 */
QString VaultDB::deriveDbKey(const QString& dbPath, const QString& userPassword) {
//    QFile fileDB(dbPath);
//    if(!fileDB.exists()) {
//        LOG_ERROR("File DB not exists!");
//        return " ";
//    }

//    QByteArray dbSalt;
//    if(fileDB.open(QIODevice::ReadOnly)) {
//        dbSalt = fileDB.read(16);
//        fileDB.close();
//    }

//    // LOG_DEBUG("Salt hex: " + dbSalt.toHex());
//    if (dbSalt.size() < 16) {
//        LOG_ERROR("File is too small, not enought 16 byte for Salt!");
//        return " ";
//    }

//    // Scrypt to create KEK
//    QByteArray KEK = scrypt(userPassword, dbSalt, n_crypt, r_crypt, p_crypt, KEY_LEN);
//    // qDebug() << KEK.toHex();

//    // HKDF - bi loi, hien tai khong su dung
//    // QByteArray DB_PassWord_Key = HKDF_SHA256(KEY_SIZE_HKDF, KEK, INFO_HKDF, SALT_HKDF);
//    // qDebug() << DB_PassWord_Key.toHex();

//    return KEK.toHex();
    return "";
}

/**
 * @brief Init all tables used in DB
 * @return
 */
bool VaultDB::initDB(void)
{
    if(!_db_.createTable<Node>()) {
        LOG_ERROR("Create Table Node Failed!");
        return false;
    }

    if(!_db_.createTable<Bucket>()){
        LOG_ERROR("Create Table Bucket Failed!");
        return false;
    }

    if(!_db_.createTable<NodeData>()){
        LOG_ERROR("Create Table NodeData Failed!");
        return false;
    }

    if(!_db_.createTable<DelNode>()){
        LOG_ERROR("Create Table DelNode Failed!");
        return false;
    }

    if(!_db_.createTable<DelSlices>()){
        LOG_ERROR("Create Table DelSlices Failed!");
        return false;
    }
    if(!_db_.createTable<XAttr>()){
        LOG_ERROR("Create Table XAttr Failed!");
        return false;
    }

    return true;
}

bool VaultDB::migrate() {
    QStringList rsListTable = _db_.getListTable(); // get list of existing table
    if(!rsListTable.isEmpty()) { // If DB has already been used
        _db_.updateTable<Node>(rsListTable);
    }

    // Initialize the DB for first time.
    int dbVersion = _db_.getVersion();

    if(dbVersion == 0) {
        if(!initDB()) {
            LOG_DEBUG("Initialize the DB for App is fail!");
            return false;
        }
        LOG_INFO("Init Vault DB sucessfull!");
        _db_.setVersion(VAULT_APP_DB_VERSION);
        return true;
    }

    // ---- Currently, don't care ----
    if(dbVersion >= VAULT_APP_DB_VERSION) {
        return true;
    }

    while (dbVersion < VAULT_APP_DB_VERSION) {
        dbVersion++;
        if(!migrateTo(dbVersion)) {
            return false;
        }
        _db_.setVersion(dbVersion);
    }

    return true;
}

bool VaultDB::migrateTo(int version)
{
    switch (version) {
    case 2:
        return migrateV1ToV2();
    default:
        return false;
    }
}

// add start 20260417
QByteArray VaultDB::getVaultKEK()
{
    return m_kek;
}
// add end 20260417

// add start 20260417
bool VaultDB::changeVaultPassword(const QString& newPassword) {
    if (_db_.connection() == nullptr) {
        LOG_ERROR("Cannot change password: DB is not open!");
        return false;
    }

    // 1. Tính toán KEK mới từ mật khẩu mới và Salt hiện tại
    QString newKeyDB = deriveDbKey(currentDBPath, newPassword);
    if(newKeyDB.trimmed().isEmpty()) {
        LOG_ERROR("newKeyDB Empty!");
        return false;
    }

    // 2. Chạy lệnh PRAGMA rekey của SQLCipher
    QString rekeyQuery = QString("PRAGMA rekey = \"x'%1'\";").arg(newKeyDB);

    char *zErrMsg = 0;
    int rc = sqlite3_exec(_db_.connection(), rekeyQuery.toUtf8().data(), NULL, NULL, &zErrMsg);

    if(rc != SQLITE_OK) {
        LOG_ERROR(QString("Rekey Error: %1").arg(zErrMsg));
        sqlite3_free(zErrMsg);
        return false;
    }

    LOG_INFO("Password changed (Rekey) successfully!");

    // Cập nhật lại m_kek trên RAM
    m_kek = newKeyDB.toUtf8();
    return true;
}
// add end 20260417

// add start 20260420
bool VaultDB::openWithKEK(const QString& dbPath, const QString& kekHex) {
    if (!QFile::exists(dbPath)) {
        LOG_ERROR("Database file does not exist: " + dbPath);
        return false;
    }

    currentDBPath = dbPath;

    EzSql::DataBase::OpenParams params;
    params.dbPath = dbPath;
    params.flags  = SQLITE_OPEN_READWRITE | SQLITE_OPEN_FULLMUTEX;

    // Định dạng mã KEK thành chuỗi hex chuẩn của SQLCipher: x'...'
    QString formattedKey = QString("x'%1'").arg(kekHex);
    params.tryKeys = QStringList() << formattedKey;

    if (_db_.open(params)) {
        // Gọi createTable thử để ép SQLCipher đọc dữ liệu thật (để đảm bảo KEK chính xác 100%)
        _db_.createTable<VaultConfig>();

        // Lưu KEK hợp lệ vào bộ nhớ tạm
        m_kek = kekHex.toUtf8();
        return true;
    }

    LOG_ERROR("Mở DB bằng KEK thất bại (KEK sai hoặc DB hỏng).");
    return false;
}
// add end 20260420

