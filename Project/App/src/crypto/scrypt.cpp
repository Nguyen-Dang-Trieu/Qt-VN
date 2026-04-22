#include "scrypt.h"

QByteArray scrypt(const QString &password, const QByteArray &salt, uint64_t n, uint64_t r, uint64_t p, size_t dkLen)
{
    QByteArray pwdBytes = password.toUtf8();
    QByteArray derivedKey(dkLen, 0);

    uint64_t maxmem = 0x7fffffff;

    int success = EVP_PBE_scrypt(
        pwdBytes.constData(),
        pwdBytes.size(),
        reinterpret_cast<const unsigned char*>(salt.constData()),
        salt.size(),
        n,
        r,
        p,
        maxmem,
        reinterpret_cast<unsigned char*>(derivedKey.data()),
        derivedKey.size()
    );

    // EVP_PBE_scrypt trả về 1 nếu thành công
    if (success != 1) {
        // Lấy lỗi từ OpenSSL để debug nếu cần
        unsigned long err = ERR_get_error();
        char errBuf[256];
        ERR_error_string_n(err, errBuf, sizeof(errBuf));

        throw std::runtime_error(std::string("OpenSSL 1.1.1 Scrypt failed: ") + errBuf);
    }

    return derivedKey;
}
