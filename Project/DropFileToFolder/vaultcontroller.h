#ifndef VAULTCONTROLLER_H
#define VAULTCONTROLLER_H

#include <QObject>
#include <QString>

class VaultController : public QObject
{
    Q_OBJECT
public:
    explicit VaultController(QObject *parent = nullptr);

    // Hàm Q_INVOKABLE để QML gọi. Xử lý được cả File và Folder
    Q_INVOKABLE bool copyPathToVault(QString sourceUrl, QString vaultName);

private:
    // Hàm đệ quy hỗ trợ copy toàn bộ Folder
    bool copyRecursively(const QString &srcFilePath, const QString &tgtFilePath);
};

#endif // VAULTCONTROLLER_H
