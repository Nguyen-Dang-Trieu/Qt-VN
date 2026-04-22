#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <QObject>
#include <QString>
#include <QJsonObject>
#include "PathDB.h"

class CAuthManager : public QObject {
    Q_OBJECT
public:
    explicit CAuthManager(CPathDB *pathDB, QObject *parent = nullptr);

    Q_INVOKABLE bool handleSignUp(const QString &username, const QString &password);
    Q_INVOKABLE bool handleLogin(const QString &username, const QString &password);
    Q_INVOKABLE bool isAccountExists(const QString &username);
    Q_INVOKABLE QString getActiveUsername();
    // add start 20260420
    Q_INVOKABLE bool verifyAccountKEK(const QString &username, const QString &kekHex);
    Q_INVOKABLE QJsonObject resetAccountPasswordWithKEK(const QString &username, const QString &kekHex, const QString &newPass);
    Q_INVOKABLE QJsonObject changeAccountPassword(const QString &currentPass, const QString &newPass);
    // add end 20260420

private:
    CPathDB *m_pathDB;
};

#endif // AUTHMANAGER_H
