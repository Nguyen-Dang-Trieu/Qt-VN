/****************************************************************************
** Meta object code from reading C++ file 'vaultcontroller.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../cryptovault_v2/Controller/vaultcontroller.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vaultcontroller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VaultController_t {
    QByteArrayData data[18];
    char stringdata0[194];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VaultController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VaultController_t qt_meta_stringdata_VaultController = {
    {
QT_MOC_LITERAL(0, 0, 15), // "VaultController"
QT_MOC_LITERAL(1, 16, 17), // "createVaultFolder"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 9), // "vaultName"
QT_MOC_LITERAL(4, 45, 8), // "password"
QT_MOC_LITERAL(5, 54, 4), // "path"
QT_MOC_LITERAL(6, 59, 17), // "importDataToVault"
QT_MOC_LITERAL(7, 77, 10), // "sourcePath"
QT_MOC_LITERAL(8, 88, 19), // "exportFileFromVault"
QT_MOC_LITERAL(9, 108, 6), // "fileId"
QT_MOC_LITERAL(10, 115, 8), // "destPath"
QT_MOC_LITERAL(11, 124, 8), // "fileName"
QT_MOC_LITERAL(12, 133, 10), // "loadVaults"
QT_MOC_LITERAL(13, 144, 11), // "deleteVault"
QT_MOC_LITERAL(14, 156, 9), // "vaultPath"
QT_MOC_LITERAL(15, 166, 11), // "renameVault"
QT_MOC_LITERAL(16, 178, 7), // "oldPath"
QT_MOC_LITERAL(17, 186, 7) // "newName"

    },
    "VaultController\0createVaultFolder\0\0"
    "vaultName\0password\0path\0importDataToVault\0"
    "sourcePath\0exportFileFromVault\0fileId\0"
    "destPath\0fileName\0loadVaults\0deleteVault\0"
    "vaultPath\0renameVault\0oldPath\0newName"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VaultController[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags
       1,    3,   44,    2, 0x02 /* Public */,
       6,    3,   51,    2, 0x02 /* Public */,
       8,    3,   58,    2, 0x02 /* Public */,
      12,    0,   65,    2, 0x02 /* Public */,
      13,    1,   66,    2, 0x02 /* Public */,
      15,    2,   69,    2, 0x02 /* Public */,

 // methods: parameters
    QMetaType::Bool, QMetaType::QString, QMetaType::QString, QMetaType::QString,    3,    4,    5,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString, QMetaType::QString,    7,    3,    4,
    QMetaType::Bool, QMetaType::Int, QMetaType::QString, QMetaType::QString,    9,   10,   11,
    QMetaType::QVariantList,
    QMetaType::Bool, QMetaType::QString,   14,
    QMetaType::QString, QMetaType::QString, QMetaType::QString,   16,   17,

       0        // eod
};

void VaultController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VaultController *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { bool _r = _t->createVaultFolder((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 1: { bool _r = _t->importDataToVault((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 2: { bool _r = _t->exportFileFromVault((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 3: { QVariantList _r = _t->loadVaults();
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 4: { bool _r = _t->deleteVault((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 5: { QString _r = _t->renameVault((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject VaultController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_VaultController.data,
    qt_meta_data_VaultController,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *VaultController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VaultController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VaultController.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int VaultController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
