#include <QStandardPaths>
#include <QFileInfo>
#include <QDir>
#include <QUuid>

#include "ezsql.h"
#include "logger.h"

#include "PathDB.h"
#include "Tbl_PathVault.h"

#define CPATH_APP_DB_VERSION        1
#define HARDCODED_PATH_DB_PASSWORD "CryptVault_PathDB_Secret_2026!@"

CPathDB::CPathDB(QObject *parent) : QObject(parent) {
    User_InfoVaults.setDb(&_db_); // Xac dinh table Path nam trong DB nao
    createPathDB();
}

CPathDB::~CPathDB() {
    close();
}

void CPathDB::createPathDB(void) {
    QString homePath      = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QString hiddenDirPath = homePath + "/.cryptvault";
    QString configDbPath  = hiddenDirPath + "/path.db";

    QDir dir;
    if (!dir.exists(hiddenDirPath)) {
        if (dir.mkpath(hiddenDirPath)) {
            LOG_INFO("Created hidden directory at: " + hiddenDirPath);
        } else {
            LOG_ERROR("Could not create hidden directory!");
            return;
        }
    }

    if (open(configDbPath)) {
        LOG_INFO("Initialized and connected to path.db successfully");
    } else {
        LOG_ERROR("SQLCipher configuration error for path.db");
    }
}


bool CPathDB::open(const QString& dbPath) {
    currentDBPath = dbPath;

    EzSql::DataBase::OpenParams params;
    params.dbPath  = dbPath;
    params.flags   = SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE | SQLITE_OPEN_FULLMUTEX;
    params.tryKeys = QStringList() << "dummy_key"; // Key empty

//    if (!QFile::exists(dbPath)) { // Check File DB
//        // 1. Generate with a random temporary Key
//        QString tempKey = QUuid::createUuid().toString();
//        params.tryKeys =  QStringList() << tempKey;

//        if (!_db_.open(params)) return false;

//        // 2. Create Table để SQLite sinh Salt
//        _db_.createTable<CSavedVault>();

//        QString currentKeyDB = deriveDbKey(dbPath);
//        if(currentKeyDB.trimmed().isEmpty()) {
//            LOG_ERROR("currentKeyDB Empty!");
//            return false;
//        }

//        LOG_WARNING("Successfully - Key DB of PathDB: " + currentKeyDB);
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

//    QString realKey = deriveDbKey(dbPath);
//    QString formattedKey = QString("x'%1'").arg(realKey);
//    params.tryKeys = QStringList() << formattedKey;

    if (!_db_.open(params)) {
        LOG_ERROR("Failed to open Path DB");
        return false;
    }

    migrate();

    return true;
}

void CPathDB::close() {
    if (_db_.connection() != nullptr) {
        _db_.close();
    }
}

bool CPathDB::migrate() {
    QStringList rsListTable = _db_.getListTable();
    // get list of existing table
    if(!rsListTable.isEmpty()) {
        _db_.updateTable<CSavedVault>(rsListTable);
    }

    int dbVersion = _db_.getVersion();
    if(dbVersion == 0) {
        if(!initDB()) {
            return false;
        }
        _db_.setVersion(CPATH_APP_DB_VERSION);
        return true;
    }

    if(dbVersion >= CPATH_APP_DB_VERSION) {
        return true;
    }

    while (dbVersion < CPATH_APP_DB_VERSION) {
        dbVersion++;
        if(!migrateTo(dbVersion)) {
            return false;
        }
        _db_.setVersion(dbVersion);
    }

    return true;
}

bool CPathDB::initDB(void) {
    if(!_db_.createTable<CSavedVault>()) {
        LOG_ERROR("Failed to create User_InfoVaults table!");
        return false;
    }
    return true;
}

bool CPathDB::migrateTo(int version) {
    switch (version) {
    case 2:
        return migrateV1ToV2();
    default:
        return false;
    }
}


/* ---------- Operations with Table User_InfoVaults --- */

QString CPathDB::getVaultPathByName(const QString &name) {
    if (!_db_.connection()) return "";

    QString safeName = name;
    safeName.replace("'", "''");

    QString query = QString("WHERE name = '%1'").arg(safeName);
    QVector<CSavedVault> records = User_InfoVaults.fetch(query);

    if (records.isEmpty()) {
        LOG_WARNING("Vault not found with name: " + name);
        return "";
    }

    return records.first().path;
}

// INSERT
bool CPathDB::saveVaultFolder(const QString &name, const QString &path) {
    if (!_db_.connection()) return false;

    // Check exists
    auto existing = User_InfoVaults.fetch(QString("WHERE name = '%1'").arg(name)); // SELECT * FROM User_InfoVaults WHERE path = '...'
    if (!existing.isEmpty()) return true;

    // Tao obj de luu tru
    CSavedVault obj_VaultFolder;
    obj_VaultFolder.name = name;
    obj_VaultFolder.path = path;

    return User_InfoVaults.insert(obj_VaultFolder);
}

// READ ALL DATA
QVariantList CPathDB::loadVaultFolders() {
    QVariantList vaultList;
    if (!_db_.connection()) {
        LOG_ERROR("Config database is not open, cannot load Vaults!");
        return vaultList;
    }

    // Retrieve all data in Table User_InfoVaults;
    QVector<CSavedVault> records = User_InfoVaults.fetch("");

    for (const auto& record : qAsConst(records)) {
        QVariantMap vaultInfo;
        // Note: name, path, icon la cac key trong QML
        vaultInfo["name"] = record.name;
        vaultInfo["path"] = record.path;
        vaultInfo["icon"] = "🔐";

        vaultList.append(vaultInfo);
    }

    LOG_INFO("Successfully loaded " + QString::number(vaultList.size()) + " Vaults from DB.");
    return vaultList;
}

bool CPathDB::deleteVaultFolder(const QString &vaultFullPath) {
    if (!_db_.connection()) return false;

    // 1. Chống lỗi SQL nếu tên thư mục có dấu nháy đơn (VD: Tai's Vault)
    QString safePath = vaultFullPath;
    safePath.replace("'", "''");

    // 2. Fetch record từ DB dựa trên path
    QString query = QString("WHERE path = '%1'").arg(safePath);
    QVector<CSavedVault> records = User_InfoVaults.fetch(query);

    if (records.isEmpty()) {
        LOG_WARNING("Vault not found in DB to delete: " + vaultFullPath);
        return false;
    }

    // 3. Sử dụng hàm remove() có sẵn của thư viện EzSql
    if (User_InfoVaults.remove(records.first())) {
        LOG_INFO("Vault deleted from DB: " + vaultFullPath);
        return true;
    }

    LOG_ERROR("Error deleting Vault from DB using EzSql::remove()");
    return false;
}

bool CPathDB::updateVaultFolder(const QString &oldPath, const QString &newPath) {
    if (!_db_.connection()) return false;

    QVector<CSavedVault> vaults = User_InfoVaults.fetch(QString("WHERE path = '%1'").arg(oldPath));
    if (vaults.isEmpty()) {
        LOG_ERROR("Vault not found in Path DB: " + QFileInfo(oldPath).fileName());
        return false;
    }

    CSavedVault vault = vaults.first();
    vault.name = QFileInfo(newPath).fileName();
    vault.path = newPath;
    User_InfoVaults.save(vault);

    LOG_INFO("Successfully Update: " + QFileInfo(newPath).fileName());
    return true;
}

//QString CPathDB::deriveDbKey(const QString& dbPath) {
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

//    if (dbSalt.size() < 16) {
//        LOG_ERROR("File is too small, not enought 16 byte for Salt!");
//        return " ";
//    }

//    // Scrypt to create KEK (Sử dụng mật khẩu Hardcode)
//    QByteArray KEK = scrypt(HARDCODED_PATH_DB_PASSWORD, dbSalt, n_crypt, r_crypt, p_crypt, KEY_LEN);
//    return KEK.toHex();
//}
