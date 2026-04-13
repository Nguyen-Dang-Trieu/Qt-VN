#include "Tbl_Node.h"

Node::Node() {}

void Node::fields()
{
    // Name Table in DB
    table("Node");

    // Define columns
    Id(m_id, "id", "INTEGER", "PRIMARY KEY AUTOINCREMENT");
    field(m_type,   "type");
    field(m_mode,   "mode");
    field(m_atime,  "atime");
    field(m_time,   "time");
    field(m_length, "length");
    field(m_name,   "name");
    field(m_parent, "parent_id");
}

void Node::fromMap(const QVariantMap& map)
{
    // Read data from QVariantMap to variables in C++
    EZ_MAP(m_id,     "id");
    EZ_MAP(m_type,   "type");
    EZ_MAP(m_mode,   "mode");
    EZ_MAP(m_atime,  "atime");
    EZ_MAP(m_time,   "time");
    EZ_MAP(m_length, "length");
    EZ_MAP(m_name,   "name");
    EZ_MAP(m_parent, "parent_id");
}

bool Node::isValidId()
{
    return m_id > 0;
}
