#ifndef HKDF_H
#define HKDF_H

// doc: https://man.openbsd.org/EVP_PKEY_CTX_set1_hkdf_key

#include <QDebug>
#include <QByteArray>
//#include <openssl/evp.h>
//#include <openssl/kdf.h>
//#include <openssl/err.h>

#define KEY_SIZE_HKDF   32                          // raw - bytes
#define INFO_HKDF       "db password protection"
#define SALT_HKDF       ""

QByteArray HKDF_SHA256(const qint32 outSize, const QByteArray &sKey, const QByteArray &info, const QByteArray &salt);

#endif // HKDF_H
