#include "ezsql.h"

#include <QDebug>

#define VALIDATE_DB          \
    if (!_db) {              \
    return SQLITE_ERROR; \
    }

#define VALIDATE_DB_STMT     \
    if (!_db || !_stmt) {    \
    return SQLITE_ERROR; \
    }

#define VALIDATE_STMT \
    if (!_stmt) {     \
    return false; \
    }

#define VALIDATE_IDX                                    \
    if (idx < 0 || idx > sqlite3_column_count(_stmt)) { \
    return false;                                   \
    }

namespace EzSql {


// implement class Stmt
Stmt::Stmt(sqlite3* db) { _db = db; }

Stmt::~Stmt() {
    finalize();
}

int Stmt::prepare(const QString& query)
{
    VALIDATE_DB

            QByteArray bQuery = query.toUtf8();
    return sqlite3_prepare_v2(_db, bQuery, bQuery.size(), &_stmt, nullptr);
}

int Stmt::bind(int idx, const QByteArray& value)
{
    VALIDATE_DB_STMT

            return sqlite3_bind_blob(_stmt, idx, value, value.size(), SQLITE_TRANSIENT);
}

int Stmt::bind(int idx, const QString& value)
{
    VALIDATE_DB_STMT

            QByteArray bvalue = value.toUtf8();
    return sqlite3_bind_text(_stmt, idx, bvalue, bvalue.size(), SQLITE_TRANSIENT);
}

int Stmt::bind(int idx, qint32 value)
{
    VALIDATE_DB_STMT

            return sqlite3_bind_int(_stmt, idx, value);
}

int Stmt::bind(int idx, qint64 value)
{
    VALIDATE_DB_STMT

            return sqlite3_bind_int64(_stmt, idx, value);
}

int Stmt::bind(int idx, double value)
{
    VALIDATE_DB_STMT

            return sqlite3_bind_double(_stmt, idx, value);
}

int Stmt::step()
{
    VALIDATE_DB_STMT

            return sqlite3_step(_stmt);
}

Result Stmt::result() { return Result(_stmt); }

int Stmt::finalize()
{
    VALIDATE_DB_STMT

            int rs = sqlite3_finalize(_stmt);
    _stmt = nullptr;

    return rs;
}

int Stmt::reset()
{
    VALIDATE_DB_STMT

            return sqlite3_reset(_stmt);
}

int Stmt::cols()
{
    return sqlite3_column_count(_stmt);;
}

const char* Stmt::printError(int e)
{
    return sqlite3_errstr(e);
}

const char* Stmt::printErrorMsg()
{
    return sqlite3_errmsg(_db);
}

qint64 Stmt::count()
{
    return sqlite3_column_int64(_stmt, 0);
}


// implement class Result
Result::Result(sqlite3_stmt* stmt)
{
    _stmt = stmt;

    if (_stmt) {
        _count = sqlite3_column_count(_stmt);
        for (auto i = 0; i < _count; i++) {
            QString colName(sqlite3_column_name(_stmt, i));
            _columns.insert(colName, i);
        }
    }
}

bool Result::map(QString& value, int idx /*= 0*/)
{
    VALIDATE_STMT
            VALIDATE_IDX

            int sz = sqlite3_column_bytes(_stmt, idx);
    value = QString::fromUtf8((char*)sqlite3_column_text(_stmt, idx), sz);

    return true;
}

bool Result::map(QByteArray& value, int idx /*=0*/)
{
    VALIDATE_STMT
            VALIDATE_IDX

            int sz = sqlite3_column_bytes(_stmt, idx);
    value = QByteArray((char*)sqlite3_column_blob(_stmt, idx), sz);

    return true;
}

bool Result::map(qint32& value, int idx /*=0*/)
{
    VALIDATE_STMT
            VALIDATE_IDX

            value = sqlite3_column_int(_stmt, idx);
    return true;
}

bool Result::map(qint64& value, int idx /*=0*/)
{
    VALIDATE_STMT
            VALIDATE_IDX

            value = sqlite3_column_int64(_stmt, idx);

    return true;
}

bool Result::map(double& value, int idx /*=0*/)
{
    VALIDATE_STMT
            VALIDATE_IDX

            value = sqlite3_column_double(_stmt, idx);

    return true;
}

bool Result::map(bool& value, int idx /*=0*/)
{
    VALIDATE_STMT
            VALIDATE_IDX

            value = sqlite3_column_int(_stmt, idx);

    return true;
}

QVariantMap Result::toMap()
{
    QVariantMap rs;
    auto keys = _columns.keys();
    for (auto const& it : keys) {
        QString val;
        if (!map(val, it)) {
            return {};
        }
        rs.insert(it, val);
    }
    return rs;
}


// implement class BaseDBO
const QString& BaseDBO::tableName()
{
    fields();
    return _tableName;
}

QString BaseDBO::idName()
{
    fields();
    return _id.name();
}

QVector<TableRecordInfo> BaseDBO::listFields()
{
    fields();
    QVector<TableRecordInfo> listFields;

    for (auto const& it : _listFields) {
        const Field& field = _field[it];
        TableRecordInfo temp;
        temp.columnName = it;
        temp.sqlType = field.sqlType();
        temp.columnConstraint = field.columnConstraint();
        listFields.append(temp);
    }

    return listFields;
}

int BaseDBO::bind(Stmt& stmt, qint32 baseIdx)
{
    cleanFields();
    fields();

    int rs;
    qint32 idx = baseIdx;
    if (!_id.name().isEmpty() && !_id.columnConstraint().toUpper().contains("AUTOINCREMENT")) {
        idx += 1;
        rs = stmt.bind(idx, _id.value().toString());
        if (rs != SQLITE_OK) {
            return rs;
        }
    }

    for(auto const& it : qAsConst(_listFields)) {
        QString val = (QString)Converter(_field[it].value());
        idx += 1;
        int rs = stmt.bind(idx, val);
        if (rs != SQLITE_OK) {
            return rs;
        }
    }

    return SQLITE_OK;
}

int BaseDBO::bind(Stmt& stmt, const QStringList& columns)
{
    cleanFields();
    fields();

    QStringList cols = columns.isEmpty() ? (_listFields << _id.name()) : columns;

    for (auto const& it : qAsConst(cols)) {
        QString val;
        if (it == _id.name()) {
            val = (QString)Converter(_id.value());
        }
        else {
            val = (QString)Converter(_field[it].value());
        }
        int rs = stmt.bind(":" + it, val);
        if (rs != SQLITE_OK) {
            return rs;
        }
    }

    return SQLITE_OK;
}

QString BaseDBO::createStmt()
{
    fields();

    QString listFields = "(";
    if (!_id.name().isEmpty()) {
        listFields = listFields + _id.name() + " " + _id.sqlType() + " " + _id.columnConstraint() + ", ";
    }

    for(auto const& it : _listFields) {
        const Field& field = _field[it];
        listFields = listFields + (it + " " + field.sqlType() + " " + field.columnConstraint() + ", ");
    }
    listFields.chop(2);
    listFields += ")";
    QString stmt = "CREATE TABLE IF NOT EXISTS " + _tableName + " " + listFields;

    return stmt;
}
QString BaseDBO::insertStmt(int numRecord)
{
    fields();

    int numValues = _field.size();
    // QString returnning = "";
    QString listFields = "(";
    if (!_id.name().isEmpty()) {
        // returnning = " RETURNING " + _id.name();

        if (!_id.columnConstraint().toUpper().contains("AUTOINCREMENT")) {
            listFields = listFields + _id.name() + ", ";
            numValues += 1;
        }
    }

    for(auto const& it : qAsConst(_listFields)) {
        listFields = listFields + it + ", ";
    }
    listFields.chop(2);
    listFields += ")";

    QString values = "(";
    for (int i = 0; i < numValues; i++) {
        values = values + "?, ";
    }
    values.chop(2);
    values += ")";

    QString records;
    for (int i = 0; i < numRecord; i++) {
        records = records + values + ", ";
    }
    records.chop(2);

    // QString stmt = "INSERT INTO " + _tableName + listFields + " VALUES " + records + returnning;
    QString stmt = "INSERT INTO " + _tableName + listFields + " VALUES " + records;

    return stmt;
}



QString BaseDBO::updateStmt(const QStringList& columns)
{
    fields();

    QString listFields = "";
    if (_id.name().isEmpty()) {
        Q_ASSERT_X(false, "BaseDBO::updateStmt", "cant update dbo without id field");
    }

    if (!columns.isEmpty()) {
        for (const auto& iCol : columns) {
            if (_field.contains(iCol)) {
                listFields = listFields + QString("%1=:%1, ").arg(iCol);
            }
        }
    }
    else {
        for(auto const& it : _listFields) {
            listFields = listFields + QString("%1=:%1, ").arg(it);
        }
    }
    listFields.chop(2);

    QString stmt = "UPDATE " + _tableName + " SET " + listFields + " WHERE " + QString("%1=:%1").arg(_id.name());

    return stmt;
}

QString BaseDBO::removeStmt()
{
    fields();

    if (_id.name().isEmpty()) {
        Q_ASSERT_X(false, "BaseDBO::updateStmt", "cant remove dbo without id field");
    }

    QString stmt = QString("DELETE FROM %1 WHERE %2=:%2").arg(_tableName, _id.name());
    return stmt;
}

QString BaseDBO::dropStmt() { return "DROP TABLE IF EXISTS " + _tableName; }

QStringList BaseDBO::allFieldName()
{
    return _listFields;
}

void BaseDBO::table(const QString& name) { _tableName = name; }

void BaseDBO::cleanFields()
{
    _field.clear();
    _id = {};
    _listFields.clear();
}

QVariantMap BaseDBO::toMap()
{
    cleanFields();
    fields();

    QVariantMap rs;
    if (!_id.name().isEmpty()) {
        rs.insert(_id.name(), _id.value());
    }

    for(auto const& it : qAsConst(_listFields)) {
        rs.insert(it, _field[it].value());
    }

    return rs;
}

void BaseDBO::set(Result& result) { fromMap(result.toMap()); }


// implement class DataBase
DataBase::~DataBase()
{
    close();
}

bool DataBase::open(const OpenParams& params)
{
    if (params.dbPath.isEmpty() || !params.flags) {
        return false;
    }

    QByteArray dbPath = params.dbPath.toUtf8();
    if (sqlite3_open_v2(dbPath, &_db, params.flags, nullptr) != SQLITE_OK) {
        _db = nullptr;
        return false;
    }

    QString validKey = "";
    for (const auto& key : params.tryKeys) {
        char* mesgError = nullptr;
        auto bkey = key.toUtf8();
        int rs = sqlite3_key(_db, bkey, bkey.size());
        if (rs == SQLITE_OK) {
            QByteArray queryOption =
                    "PRAGMA journal_mode = WAL;"
                    "PRAGMA synchronous = NORMAL;"
                    "PRAGMA TEMP_STORE = MEMORY;"
                    "PRAGMA cache_size = 10000;"
                    "PRAGMA page_size = 4096;"
                    "PRAGMA recursive_triggers = 1;";

            rs = sqlite3_exec(_db, queryOption, nullptr, nullptr, &mesgError);
            if (rs == SQLITE_OK) {
                validKey = key;
                break;
            }
            else {
                sqlite3_free(mesgError);
            }
        }
        else {
            sqlite3_free(mesgError);
        }
    }

    if(validKey != "") {
        if(params.newKey != "" && params.newKey != validKey) {
            QByteArray bnewkey = params.newKey.toUtf8();
            int rs = sqlite3_rekey(_db, bnewkey, bnewkey.size());

            if(rs == SQLITE_OK) {
                return true;
            }
            else {
                close();
                return false;
            }
        }
    }
    else {
        close();
        return false;
    }

    return true;
}

void DataBase::close()
{
    if (_db) {
        sqlite3_close_v2(_db);
        _db = nullptr;
    }
}

int DataBase::getVersion()
{
    if(!_db) {
        return -1;
    }

    Stmt stmt(_db);
    if (stmt.prepare("PRAGMA user_version") == SQLITE_OK && stmt.step() == SQLITE_ROW) {
        Result result = stmt.result();
        if (result.count() > 0) {
            int version = -1;
            result.map(version);
            return version;
        }
    }

    return -1;
}

QStringList DataBase::getListTable()
{
    if(!_db) {
        ezsqlError << "connection invalid";
        return {};
    }

    QString query = "SELECT * FROM sqlite_master where type='table';";
    char* mesgError = nullptr;
    int rs = 0;
    QStringList tables;
    rs = sqlite3_exec(_db, query.toUtf8().data(), [](void* data, int argc, char** argv, char** azColName) -> int {
        QStringList* tables = (QStringList*)(data);
        for (auto i = 0; i < argc; i++) {
            if(azColName[i] && std::strcmp(azColName[i], "tbl_name") == 0) {
                *tables << QString(argv[i]);
            }
        }

        return 0;

    }, &tables, &mesgError);

    if(rs == SQLITE_OK) {
        return tables;
    }
    else {
        ezsqlError << "get list table error [" << rs << "]" << mesgError;
        sqlite3_free(mesgError);
        return tables;
    }
}

bool DataBase::setVersion(int version)
{
    if (!_db) {
        return false;
    }

    Stmt stmt(_db);
    return stmt.prepare(QString("PRAGMA user_version=%1").arg(version)) == SQLITE_OK && stmt.step() == SQLITE_DONE;
}


// implement class Converter
Converter::Converter(const QVariant& val) { _val = val; }

Converter::operator int() const { return _val.toInt(); }
Converter::operator uint() const { return _val.toUInt(); }

Converter::operator bool() const { return _val.toBool(); }

Converter::operator long() const { return _val.toFloat(); }

Converter::operator qint64() const { return _val.toLongLong(); }

Converter::operator quint64() const { return _val.toULongLong(); }

Converter::operator double() const { return _val.toDouble(); }

Converter::operator QByteArray() const { return _val.toByteArray(); }

Converter::operator QVariant() const { return _val; }

Converter::operator QString() const
{
    if (_val.type() == QVariant::List || _val.type() == QVariant::StringList || _val.type() == QVariant::Map ||
            _val.type() == QVariant::Hash) {
        return QJsonDocument::fromVariant(_val).toJson(QJsonDocument::Compact);
    }
    else if (_val.type() == QVariant::Bool) {
        return _val.toBool() ? "1" : "0";
    }
    return _val.toString();
}

Converter::operator QVariantList() const
{
    if (_val.type() == QVariant::List || _val.type() == QVariant::StringList) {
        return _val.toList();
    }
    else if (_val.type() == QVariant::String) {
        return QJsonDocument::fromJson(_val.toString().toUtf8()).toVariant().toList();
    }
    return {};
}

Converter::operator QStringList() const
{
    if (_val.type() == QVariant::List || _val.type() == QVariant::StringList) {
        return _val.toStringList();
    }
    else if (_val.type() == QVariant::String) {
        return QJsonDocument::fromJson(_val.toString().toUtf8()).toVariant().toStringList();
    }
    return {};
}

Converter::operator QVariantMap() const
{
    if (_val.type() == QVariant::String) {
        return QJsonDocument::fromJson(_val.toString().toUtf8()).toVariant().toMap();
    }
    return _val.toMap();
}

Converter::operator QVariantHash() const
{
    if (_val.type() == QVariant::String) {
        return QJsonDocument::fromJson(_val.toString().toUtf8()).toVariant().toHash();
    }
    return _val.toHash();
}


// implement class Field
const QString& Field::name() const { return _name; }

const QString& Field::sqlType() const { return _sqlType; }

const QString& Field::columnConstraint() const { return _columnConstraint; }

const QVariant& Field::value() const { return _value; }

QString Field::sqlTypeFrom(const QString&) { return "TEXT"; }

QString Field::sqlTypeFrom(const QStringList&) { return "TEXT"; }

QString Field::sqlTypeFrom(const QVariantList&) { return "TEXT"; }

QString Field::sqlTypeFrom(const QVariantMap&) { return "TEXT"; }

QString Field::sqlTypeFrom(const QByteArray&) { return "BLOB"; }

QString Field::sqlTypeFrom(const qint32&) { return "INTEGER"; }
QString Field::sqlTypeFrom(const quint32&) { return "INTEGER"; }
QString Field::sqlTypeFrom(const qint64&)  { return "INTEGER"; }
QString Field::sqlTypeFrom(const quint64&) { return "INTEGER"; }
QString Field::sqlTypeFrom(const double&) { return "REAL"; }

QString Field::sqlTypeFrom(const bool&) { return "INTEGER"; }

}  // namespace EzSql
