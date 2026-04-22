#include "Tbl_Xattr.h"

XAttr::XAttr() { }

void XAttr::fields()
{
     table("xattr");

    Id(xattr_id, "id", "INTEGER", "PRIMARY KEY AUTOINCREMENT");

    field(xattr_nodeId, "node_id", "INTEGER",   "NOT NULL");
    field(xattr_name,   "name",    "TEXT",      "NOT NULL");
    field(xattr_value,  "value",   "BLOB",      "NOT NULL");
}

void XAttr::fromMap(const QVariantMap &map)
{
    EZ_MAP(xattr_id, "id");
    EZ_MAP(xattr_nodeId, "node_id");
    EZ_MAP(xattr_name, "name");

    if (map.contains("value")) {
        xattr_value = (QByteArray)EzSql::Converter(map["value"]);
    }
}

bool XAttr::isValidId()
{
    return xattr_id > 0;
}
