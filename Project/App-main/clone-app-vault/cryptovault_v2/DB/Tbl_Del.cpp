#include "Tbl_Del.h"

DelNode::DelNode() {
    table("delnode");
}

void DelNode::fields() {
    table("delnode");
    Id(m_inode, "inode", "INTEGER", "PRIMARY KEY");
    field(m_expireAt, "expire_at", "INTEGER");
}

void DelNode::fromMap(const QVariantMap &map) {
    EZ_MAP(m_inode, "inode");
    EZ_MAP(m_expireAt, "expire_at");
}

bool DelNode::isValidId() {
    return m_inode > 0;
}

DelSlices::DelSlices() {
    table("delslices");
}

void DelSlices::fields() {
    Id(m_sliceUuid, "slice_uuid", "TEXT", "UNIQUE");
    field(m_bucketId, "bucket_id", "INTEGER", "NOT NULL");
}

void DelSlices::fromMap(const QVariantMap &map) {
    EZ_MAP(m_sliceUuid, "slice_uuid");
    EZ_MAP(m_bucketId, "bucket_id");
}

bool DelSlices::isValidId() {
    return !m_sliceUuid.isEmpty();
}
