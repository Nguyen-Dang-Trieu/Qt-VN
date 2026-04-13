#include "aes_256_gcm.h"
#include <QDebug>

// Hàm hỗ trợ in lỗi nội bộ
void handle_openssl_errors() {
    unsigned long errCode;
    while ((errCode = ERR_get_error())) {
        qCritical() << "OpenSSL Error:" << ERR_error_string(errCode, nullptr);
    }
}

bool aes_gcm_encrypt(const QByteArray &plaintext,
                    const QByteArray &key,
                    const QByteArray &iv,
                    const QByteArray &aad,
                    QByteArray &ciphertext,
                    QByteArray &tag)
{
    EVP_CIPHER_CTX *ctx = nullptr;
    int len = 0;
    int ciphertext_len = 0;
    bool success = false;

    // Chuẩn bị kích thước buffer
    ciphertext.resize(plaintext.size());
    tag.resize(16);

    if (!(ctx = EVP_CIPHER_CTX_new())) goto err;

    // 1. Khởi tạo thuật toán
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr)) goto err;

    // 2. Thiết lập độ dài IV
    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, iv.size(), nullptr)) goto err;

    // 3. Khởi tạo Key và IV
    if (1 != EVP_EncryptInit_ex(ctx, nullptr, nullptr,
                                reinterpret_cast<const unsigned char*>(key.constData()),
                                reinterpret_cast<const unsigned char*>(iv.constData()))) goto err;

    // 4. Thêm AAD (Dữ liệu xác thực bổ sung - không bắt buộc)
    if (!aad.isEmpty()) {
        if (1 != EVP_EncryptUpdate(ctx, nullptr, &len,
                                   reinterpret_cast<const unsigned char*>(aad.constData()),
                                   aad.size())) goto err;
    }

    // 5. Mã hóa Plaintext
    if (1 != EVP_EncryptUpdate(ctx, reinterpret_cast<unsigned char*>(ciphertext.data()), &len,
                               reinterpret_cast<const unsigned char*>(plaintext.constData()),
                               plaintext.size())) goto err;
    ciphertext_len = len;

    // 6. Kết thúc mã hóa
    if (1 != EVP_EncryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(ciphertext.data()) + len, &len)) goto err;
    ciphertext_len += len;

    // 7. Lấy TAG
    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16,
                                 reinterpret_cast<unsigned char*>(tag.data()))) goto err;

    success = true;

err:
    if (!success) handle_openssl_errors();
    if (ctx) EVP_CIPHER_CTX_free(ctx);
    return success;
}

QByteArray aes_gcm_decrypt(const QByteArray &ciphertext,
                          const QByteArray &tag,
                          const QByteArray &key,
                          const QByteArray &iv,
                          const QByteArray &aad)
{
    EVP_CIPHER_CTX *ctx = nullptr;
    int len = 0;
    int plaintext_len = 0;
    QByteArray plaintext;

    plaintext.resize(ciphertext.size());

    if (!(ctx = EVP_CIPHER_CTX_new())) goto err;

    // 1. Khởi tạo thuật toán
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr)) goto err;

    // 2. Thiết lập độ dài IV
    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, iv.size(), nullptr)) goto err;

    // 3. Khởi tạo Key và IV
    if (1 != EVP_DecryptInit_ex(ctx, nullptr, nullptr,
                                reinterpret_cast<const unsigned char*>(key.constData()),
                                reinterpret_cast<const unsigned char*>(iv.constData()))) goto err;

    // 4. Thêm AAD
    if (!aad.isEmpty()) {
        if (1 != EVP_DecryptUpdate(ctx, nullptr, &len,
                                   reinterpret_cast<const unsigned char*>(aad.constData()),
                                   aad.size())) goto err;
    }

    // 5. Giải mã
    if (1 != EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char*>(plaintext.data()), &len,
                               reinterpret_cast<const unsigned char*>(ciphertext.constData()),
                               ciphertext.size())) goto err;
    plaintext_len = len;

    // 6. Thiết lập TAG để kiểm tra tính toàn vẹn
    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16,
                                 const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(tag.constData())))) goto err;

    // 7. Kết thúc giải mã (Bước quan trọng để xác thực TAG)
    if (EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(plaintext.data()) + len, &len) <= 0) {
        qWarning() << "GCM Decrypt failed: Tag authentication failed!";
        goto err;
    }

    plaintext_len += len;
    plaintext.resize(plaintext_len); // Cắt bỏ các byte thừa nếu có

    if (ctx) EVP_CIPHER_CTX_free(ctx);
    return plaintext;

err:
    handle_openssl_errors();
    if (ctx) EVP_CIPHER_CTX_free(ctx);
    return QByteArray(); // Trả về rỗng nếu có lỗi
}
