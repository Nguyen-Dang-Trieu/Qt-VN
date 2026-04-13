#ifndef AES_256_GCM_H
#define AES_256_GCM_H

#include <QByteArray>
#include <openssl/evp.h>
#include <openssl/err.h>

bool aes_gcm_encrypt(const QByteArray &plaintext,
                    const QByteArray &key,
                    const QByteArray &iv,
                    const QByteArray &aad,
                    QByteArray &ciphertext,
                    QByteArray &tag);

QByteArray aes_gcm_decrypt(const QByteArray &ciphertext,
                          const QByteArray &tag,
                          const QByteArray &key,
                          const QByteArray &iv,
                          const QByteArray &aad);


#endif // AES_256_GCM_H
