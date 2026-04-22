#ifndef SCRYPT_H
#define SCRYPT_H

// Use OpenSSL version 1.1.1
#include <openssl/evp.h>
#include <openssl/err.h>
#include <QString>
#include <QByteArray>
#include <stdexcept>

// Define the params for scrypt
#define n_crypt  65536
#define r_crypt  8
#define p_crypt  1
#define KEY_LEN  32   // Bytes


QByteArray scrypt(const QString &password, const QByteArray &salt, uint64_t n, uint64_t r, uint64_t p, size_t dkLen);


#endif // SCRYPT_H
