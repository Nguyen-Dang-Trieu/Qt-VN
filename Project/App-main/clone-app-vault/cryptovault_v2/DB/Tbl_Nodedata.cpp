#include "Tbl_Nodedata.h"

NodeData::NodeData() {}

void NodeData::fields()
{
    table("node_data");
    Id(id, "id", "INTEGER", "PRIMARY KEY AUTOINCREMENT");
    field(node_id, "node_id", "INTEGER", "NOT NULL");
    field(bucket_id, "bucket_id", "INTEGER", "NOT NULL");
    field(slice_uuid, "slice_uuid", "TEXT", "NOT NULL");
    field(slice_number, "slice_number", "INTEGER", "NOT NULL");

}

void NodeData::fromMap(const QVariantMap &map)
{
    EZ_MAP(id, "id");
    EZ_MAP(node_id, "node_id");
    EZ_MAP(bucket_id, "bucket_id");
    EZ_MAP(slice_uuid, "slice_uuid");
    EZ_MAP(slice_number, "slice_number");
}

bool NodeData::isValidId()
{
    return node_id > 0;
}
