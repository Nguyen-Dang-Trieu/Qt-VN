#include "model.h"

// add start 20260415
bool NodeModel::checkPassword(const QString &vaultName, const QString &passVault, const QString &pathDB) {

//   VaultDB vDB;
//   QString fullPathDB = pathDB + "/" + vaultName + ".db";
//   bool isDbReady = vDB.open(fullPathDB, passVault);
//   if (isDbReady) {
//       vDB.close();
//       return true;
//   }
//   return false;
}
// add end 20260415

// Add new: 2026-04-22-drew
QString NodeModel::getVaultKEK(const QString &vaultName, const QString &passVault, const QString &pathDB){
    VaultDB vDB;

   LOG_DEBUG("[MODEL]" + pathDB);
   LOG_DEBUG("[MODEL]" + vaultName);
   LOG_DEBUG("[MODEL]" + passVault);

   QString fullPathDB = getDbPathFromVaultFolder(pathDB);
   LOG_DEBUG("[MODEL]" + fullPathDB);

    if (vDB.open(fullPathDB, passVault)) {
        QString kek = vDB.getVaultKEK();
        vDB.close();
        return kek;
    }
    return "ERROR_GETTING_KEK";
}
// add end

// add start 20260420
QString NodeModel::getAccountKEK(const QString &username, const QString &password) {
    CPathDB pDB;
    QString homePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QString dbPath   = homePath + "/.cryptvault/" + username + "/user_data.db";

    pDB.setUsernameSignup(username);
    pDB.setPasswordSignup(password);

    if (pDB.open(dbPath)) {
        QString kek = pDB.getKEK();
        pDB.close();
        return kek;
    }
    return "ERROR_GETTING_ACCOUNT_KEK";
}

// add end 20260420
