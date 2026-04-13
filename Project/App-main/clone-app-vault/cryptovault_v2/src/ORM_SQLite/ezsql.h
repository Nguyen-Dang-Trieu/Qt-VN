#ifndef EZSQL_H
#define EZSQL_H

#define ezsqlLog     qDebug() << __FUNCTION__ << ":" << __LINE__
#define ezsqlError   qDebug() << __FUNCTION__ << ":" << __LINE__ << "\033[1;37;41m[ezsql][Error]\033[0m "
#define ezsqlWarning qDebug() << __FUNCTION__ << ":" << __LINE__ << "\033[1;37;43m[ezsql][warning]\033[0m "
#define ezsqlInfo    qDebug() << __FUNCTION__ << ":" << __LINE__ << "\033[1;37;42m[ezsql][Info]\033[0m "

// #define SQLITE_HAS_CODEC - hiện tại không dùng sqlcipher
#include "sqlite3.h"
#include <cstring>

#include <QDebug>
#include <QMap>
#include <QString>
#include <QJsonDocument>
#include <QVariant>

#define EZ_MAP(field, fieldName)                                   \
    if (map.contains(fieldName)) {                                 \
        field = (decltype(field))EzSql::Converter(map[fieldName]); \
    }

struct TableRecordInfo {
    QString columnName;
    QString sqlType;
    QString columnConstraint;
    bool    isPrimaryKey = false;
};

namespace EzSql {

class Converter {
public:
    Converter(const QVariant& val);
    explicit operator int() const;
    explicit operator uint() const;
    explicit operator bool() const;
    explicit operator long() const;
    explicit operator qint64() const;
    explicit operator quint64() const;
    explicit operator double() const;
    explicit operator QByteArray() const;
    explicit operator QVariant() const;
    explicit operator QString() const;
    explicit operator QVariantList() const;
    explicit operator QStringList() const;
    explicit operator QVariantMap() const;
    explicit operator QVariantHash() const;

private:
    QVariant _val;
};

class Result {
public:
    explicit Result(sqlite3_stmt* stmt = nullptr);

    bool map(QString& value, int idx = 0);
    bool map(QByteArray& value, int idx = 0);
    bool map(qint32& value, int idx = 0);
    bool map(qint64& value, int idx = 0);
    bool map(double& value, int idx = 0);
    bool map(bool& value, int idx = 0);
    qint32 count() const { return _count; }

    template <typename T>
    bool map(T& value, const QString& name)
    {
        if (!_stmt) {
            return false;
        }

        if (!_columns.contains(name)) {
            return false;
        }

        return map(value, _columns[name]);
    }

    QVariantMap toMap();

private:
    QHash<QString, qint32> _columns;  // <colname, index>
    sqlite3_stmt* _stmt = nullptr;
    qint32 _count = 0;
};

class Stmt {
public:
    explicit Stmt(sqlite3* db = nullptr);
    ~Stmt();

    int prepare(const QString& query);

    int bind(int idx, const QByteArray& value);
    int bind(int idx, const QString& value);
    int bind(int idx, qint32 value);
    int bind(int idx, qint64 value);
    int bind(int idx, double value);

    template <typename T>
    int bind(const QString& name, const T& value)
    {
        if (!_db || !_stmt) {
            return SQLITE_ERROR;
        }

        QByteArray bName = name.toUtf8();
        int idx = sqlite3_bind_parameter_index(_stmt, bName);
        if (!idx) {
            return SQLITE_ERROR;
        }

        return bind(idx, value);
    }

    qint64 count();

    int step();
    Result result();
    int finalize();
    int reset();
    int cols();

    const char* printError(int e);
    const char* printErrorMsg();

    void db(sqlite3* db) { _db = db; }

private:
    sqlite3_stmt* _stmt = nullptr;
    sqlite3* _db = nullptr;
};

class Field {
public:
    Field() {}

    template<typename T>
    Field(const T& value, const QString& name, const QString& sqlType, const QString& constraint)
    {
        _value = value;
        _name = name;
        _sqlType = sqlType.isEmpty() ? sqlTypeFrom(value) : sqlType;
        _columnConstraint = constraint;
    }

    virtual ~Field() = default;

    const QString& name() const;
    const QString& sqlType() const;
    const QString& columnConstraint() const;
    const QVariant& value() const;

    static QString sqlTypeFrom(const QString&);
    static QString sqlTypeFrom(const QStringList&);
    static QString sqlTypeFrom(const QVariantList&);
    static QString sqlTypeFrom(const QVariantMap&);
    static QString sqlTypeFrom(const QByteArray&);
    static QString sqlTypeFrom(const qint32&);
    static QString sqlTypeFrom(const quint32&);
    static QString sqlTypeFrom(const qint64&);
    static QString sqlTypeFrom(const quint64&);
    static QString sqlTypeFrom(const double&);
    static QString sqlTypeFrom(const bool&);

protected:
    QString _name;
    QString _sqlType;
    QString _columnConstraint;
    QVariant _value;
};

class BaseDBO {
public:
    BaseDBO() {}
    virtual ~BaseDBO() = default;

    const QString& tableName();
    QString idName();
    QVector<TableRecordInfo> listFields();

    int bind(Stmt& stmt, qint32 baseIdx = 0);
    int bind(Stmt& stmt, const QStringList& columns = {});

    QString createStmt();
    QString insertStmt(int numRecord = 1);
    QString updateStmt(const QStringList& columns = {});
    QString removeStmt();
    QString dropStmt();
    QStringList allFieldName();

    QVariantMap toMap();
    virtual void fromMap(const QVariantMap& map) = 0;
    virtual bool isValidId() = 0;

    void set(Result& result);

protected:
    template<typename T>
    void field(const T& value, const QString& name, const QString& sqlType = "", const QString& constraint = "")
    {
        if (!_field.contains(name)) {
            Field field(value, name, sqlType, constraint);
            _field.insert(name, field);
            _listFields.append(name);
        }
    }

    template<typename T>
    void Id(const T& value, const QString& name, const QString& sqlType = "", const QString& constraint = "")
    {
        if (_id.name().isEmpty()) {
            Field field(value, name, sqlType, constraint);
            _id = field;
        }
    }

    void table(const QString& name);

    virtual void fields() = 0;

private:
    void cleanFields();

private:
    QStringList _listFields;
    QHash<QString, Field> _field;
    Field _id;
    QString _tableName;
};

class DataBase {
public:
    struct OpenParams {
        QString dbPath;
        QStringList tryKeys;
        QString newKey;
        int flags = 0;
    };

public:
    DataBase() = default;
    ~DataBase();
    bool open(const OpenParams& params);
    void close();
    sqlite3* connection() const { return _db; }
    virtual void onPreUpdate() {}

    int getVersion();
    bool setVersion(int version);
    QStringList getListTable();

    template<typename T>
    bool updateTable(const QStringList& listTable)
    {
        T obj;
        if(listTable.contains(obj.tableName())) {
            return autoAlterTable<T>();
        }
        else {
            return createTable<T>();
        }
    }

    template<typename T>
    bool autoAlterTable()
    {
        if (!_db) {
            ezsqlError << "connection invalid";
            return false;
        }

        QString templateAlterTb("ALTER TABLE '%1' ADD COLUMN '%2' %3;");

        QStringList rsListColInDb = getListColumn<T>();
        if(rsListColInDb.isEmpty())
            return false;

        T obj;
        auto curTableInfo = obj.listFields();
        QStringList tbListCol = rsListColInDb;
        auto tbName       = obj.tableName();

        for (const auto& iRecord : curTableInfo) {
            if (tbListCol.contains(iRecord.columnName)) {
                tbListCol.removeOne(iRecord.columnName);
            }
            else {
                char* mesgError = nullptr;
                int rs = 0;
                QString query = "";

                query = QString(templateAlterTb).arg(tbName).arg(iRecord.columnName).arg(iRecord.sqlType);

                rs = sqlite3_exec(_db, query.toUtf8().data(), nullptr, nullptr, &mesgError);
                if (rs != SQLITE_OK) {
                    ezsqlError << " alter table error [" << rs << "]" << mesgError;
                    sqlite3_free(mesgError);
                    return false;
                }
            }
        }

        if(!tbListCol.empty()) {
            //todo: remove unused column
        }

        return true;
    }

    template<typename T>
    QStringList getListColumn()
    {

        if(!_db) {
            ezsqlError << "connection invalid";
            return {};
        }

        T obj;
        QString tableName = obj.tableName();
        QString query = QString("PRAGMA table_info(%1);").arg(tableName);

        QStringList listColumns;
        char* mesgError = nullptr;
        int rs = 0;
        rs = sqlite3_exec(_db, query.toUtf8().data(), [](void* data, int argc, char** argv, char** azColName) -> int {
                QStringList* listColumns = (QStringList*)(data);
                for (auto i = 0; i < argc; i++) {
                    if(azColName[i] && std::strcmp(azColName[i], "name") == 0) {
                        *listColumns << QString(argv[i]);
                    }
                }

                return 0;

            }, &listColumns, &mesgError);

        if(rs == SQLITE_OK) {
            return listColumns;
        }
        else {
            ezsqlError << "get list table error [" << rs << "]" << mesgError;
            sqlite3_free(mesgError);
            return listColumns;
        }
    }

    template <typename T>
    bool createTable()
    {
        static_assert(std::is_base_of_v<BaseDBO, T>, "Error: type T must base on BaseDBO");

        if (!_db) {
            return false;
        }

        T tb;
        tb.fields();

        QString queryCreate = tb.createStmt();
        Stmt stmt(_db);
        if (stmt.prepare(queryCreate) != SQLITE_OK) {
            ezsqlError << "create table failed : prepare \n" << queryCreate;
            return false;
        }
        if (stmt.step() != SQLITE_DONE) {
            ezsqlError << "create table failed : step \n" << queryCreate;
            return false;
        }
        if (stmt.finalize() != SQLITE_OK) {
            return false;
        }

        return true;
    }

private:
    sqlite3* _db = nullptr;
};

template <typename T>
class DbSet {
public:
    DbSet() = default;
    void setDb(const DataBase* db) { _db = db;}
    void unsetDb() { _db = nullptr; }

    bool fetch(const QString& qury, std::function<void(Result& result)> cb) {
        if(!_db || !_db->connection()) {
            ezsqlError << "connection invalid";
            return false;
        }

        EzSql::Stmt stmt(_db->connection());
        if(stmt.prepare(qury) != SQLITE_OK) {
            ezsqlError << "fetch failed : prepare \n" << qury;
            return false;
        }

        while (1) {
            int r = stmt.step();
            if (r == SQLITE_ROW) {
                if (cb) {
                    auto res = stmt.result();
                    cb(res);
                }
            }
            else if (r == SQLITE_DONE) {
                return true;
            }
            else {
                ezsqlError << "fetch failed : step \n" << qury;
                return false;
            }
        }
    }

    QVector<T> fetch(const QString& partQuery) {
        QString tbName = T().tableName();
        QString qury = QString("select * from %1 %2").arg(tbName, partQuery);

        QVector<T> results;
        fetch(qury, [&results](Result& result) {
            T obj;
            obj.fields();
            obj.set(result);
            results.append(obj);
        });

        return results;
    }

    QVector<T> query(const QString& query) {
        QVector<T> results;
        fetch(query, [&results](Result& result) {
            T obj;
            obj.fields();
            obj.set(result);
            results.append(obj);
        });

        return results;
    }

    bool insert(T& obj)
    {
        if(!_db || !_db->connection()) {
            ezsqlError << "connection invalid";
            return false;
        }

        QString query = obj.insertStmt();
        EzSql::Stmt stmt(_db->connection());

        if (stmt.prepare(query) != SQLITE_OK) {
            ezsqlError << "prepare falied " << query;
            return false;
        }

        if (obj.bind(stmt, 0) != SQLITE_OK) {
            ezsqlError << "bind falied " << query;
            return false;
        }

        // if (stmt.step() != SQLITE_ROW)
        if (stmt.step() != SQLITE_DONE) {
            ezsqlError << "step falied " << query;
            return false;
        }

        // Result rs = stmt.result();
        sqlite3_int64 lastId = sqlite3_last_insert_rowid(_db->connection());
        // obj.set(rs);

        QVariantMap map;
        map.insert(obj.idName(), (qint64)lastId);
        obj.fromMap(map);

        return true;
    }

    bool update(T& obj, const QStringList& columns = {})
    {
        if(!_db || !_db->connection()) {
            ezsqlError << "connection invalid";
            return false;
        }

        QString query = obj.updateStmt(columns);
        EzSql::Stmt stmt(_db->connection());

        if (stmt.prepare(query) != SQLITE_OK) {
            ezsqlError << "prepare falied " << query;
            return false;
        }

        if (obj.bind(stmt, columns) != SQLITE_OK) {
            ezsqlError << "bind falied " << query;
            return false;
        }

        if (stmt.step() != SQLITE_DONE) {
            ezsqlError << "step falied " << query;
            return false;
        }

        return true;
    }

    bool save(T& obj) {
        if(!obj.isValidId()) {
            return insert(obj);
        }

        return update(obj);
    }

    bool removeAll() {
        return execQuery("DELETE FROM @table");
    }

    bool execQuery(const QString & query) {
        if(!_db || !_db->connection()) {
            ezsqlError << "connection invalid";
            return false;
        }

        T obj;
        QString qury = query;
        qury.replace("@table", obj.tableName());
        EzSql::Stmt stmt(_db->connection());
        if (stmt.prepare(qury) != SQLITE_OK) {
            ezsqlError << "prepare falied " << qury;
            return false;
        }

        if (stmt.step() != SQLITE_DONE) {
            ezsqlError << "step falied " << qury;
            return false;
        }

        if (stmt.finalize() != SQLITE_OK) {
            return false;
        }
        return true;
    }

    qint64 count(const QString & query){
        if(!_db || !_db->connection()) {
            ezsqlError << "connection invalid";
            return false;
        }

        QString tbName = T().tableName();
        QString qury = QString("select count(*) from %1 %2").arg(tbName, query);
        qint64 count = 0;
        EzSql::Stmt stmt(_db->connection());

        if (stmt.prepare(qury) != SQLITE_OK) {
            stmt.finalize();
            ezsqlError << "prepare falied " << qury;
            return count;
        }

        int s = stmt.step();
        if(s != SQLITE_DONE && s!= SQLITE_ROW) {
            stmt.finalize();
            ezsqlError << "step falied " << qury;
            return count;
        }

        int cols = stmt.cols();
        if(cols < 0){
            stmt.finalize();
            ezsqlError << "cols falied " << qury;
            return count;
        }

        count = stmt.count();

        if (stmt.finalize() != SQLITE_OK) {
            return count;
        }

        return count;
    }

    bool remove(T& obj) {
        if(!_db || !_db->connection()) {
            ezsqlError << "connection invalid";
            return false;
        }

        QString query = obj.removeStmt();
        EzSql::Stmt stmt(_db->connection());

        if (stmt.prepare(query) != SQLITE_OK) {
            ezsqlError << "prepare falied " << query;
            return false;
        }

        if (obj.bind(stmt, {obj.idName()}) != SQLITE_OK) {
            ezsqlError << "bind falied " << query;
            return false;
        }

        if (stmt.step() != SQLITE_DONE) {
            ezsqlError << "step falied " << query;
            return false;
        }

        return true;
    }


private:
    const DataBase* _db = nullptr;
};

}  // namespace EzSql

#endif  // EZSQL_H
