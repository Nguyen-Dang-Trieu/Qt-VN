#include "hkdf.h"

QByteArray HKDF_SHA256(const qint32 outSize, const QByteArray &sKey, const QByteArray &info, const QByteArray &salt)
{
//    QByteArray rs;

//    // --- BẮT BUỘC: Khởi tạo bảng thuật toán cho OpenSSL 1.1.1 ---
//    // Nếu không có 2 dòng này, EVP_PKEY_CTX_set_hkdf_md sẽ luôn thất bại
//    OpenSSL_add_all_algorithms();
//    OpenSSL_add_all_digests();
//    ERR_load_crypto_strings();

//    // Tạo Context cho HKDF
//    EVP_PKEY_CTX *pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_HKDF, nullptr);
//    if (!pctx) {
//        qDebug() << "Lỗi: Không thể tạo EVP_PKEY_CTX. Mã lỗi:" << ERR_get_error();
//        return rs;
//    }

//    // Khởi tạo derive
//    if (EVP_PKEY_derive_init(pctx) <= 0) {
//        qDebug() << "Lỗi: EVP_PKEY_derive_init thất bại";
//        EVP_PKEY_CTX_free(pctx);
//        return rs;
//    }

//    // 1. Thiết lập Digest (Dùng đúng Macro của OpenSSL 1.1.1)
//    // EVP_sha256() phải được nạp từ OpenSSL_add_all_digests() ở trên
//    if (EVP_PKEY_CTX_set_hkdf_md(pctx, EVP_sha256()) <= 0) {
//        qDebug() << "Lỗi: EVP_PKEY_CTX_set_hkdf_md thất bại. Mã lỗi:" << ERR_get_error();
//        EVP_PKEY_CTX_free(pctx);
//        return rs;
//    }

//    // 2. Thiết lập Key (IKM)
//    if (EVP_PKEY_CTX_set1_hkdf_key(pctx, (const unsigned char*)sKey.constData(), sKey.size()) <= 0) {
//        qDebug() << "Lỗi: Set Key thất bại";
//        EVP_PKEY_CTX_free(pctx);
//        return rs;
//    }

//    // 3. Thiết lập Salt (Xử lý salt rỗng theo RFC)
//    if (!salt.isEmpty()) {
//        if (EVP_PKEY_CTX_set1_hkdf_salt(pctx, (const unsigned char*)salt.constData(), salt.size()) <= 0) {
//            qDebug() << "Lỗi: Set Salt thất bại";
//            EVP_PKEY_CTX_free(pctx);
//            return rs;
//        }
//    }

//    // 4. Thiết lập Info
//    if (!info.isEmpty()) {
//        if (EVP_PKEY_CTX_add1_hkdf_info(pctx, (const unsigned char*)info.constData(), info.size()) <= 0) {
//            qDebug() << "Lỗi: Set Info thất bại";
//            EVP_PKEY_CTX_free(pctx);
//            return rs;
//        }
//    }

//    // 5. Thực hiện Derive để lấy kết quả
//    size_t outLen = static_cast<size_t>(outSize);
//    QByteArray out(outSize, 0);

//    if (EVP_PKEY_derive(pctx, (unsigned char*)out.data(), &outLen) <= 0) {
//        qDebug() << "Lỗi: EVP_PKEY_derive thực thi thất bại";
//    } else {
//        out.resize(static_cast<int>(outLen));
//        rs = out;
//    }

//    EVP_PKEY_CTX_free(pctx);
//    return rs;
}
