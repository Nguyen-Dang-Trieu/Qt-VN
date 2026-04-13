QT += quick sql
CONFIG += c++17
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Include SQLCipher libraries and headers
#include($$PWD/3rd/sqlcipher-4.5.0/SqlCipher.pri)
include($$PWD/3rd/openssl_1_1_1w/OpenSSL.pri)

# Enable SQLCipher encryption support
#DEFINES += SQLITE_HAS_CODEC

## For header compatibility, include sqlite3.h from sqlcipher
#INCLUDEPATH += $$PWD/3rd/sqlcipher-4.5.0/dist_mingw/include

win32 {
    LIBS += -luser32 -lgdi32 -lkernel32
}

INCLUDEPATH += $$PWD/3rd/sqlite3

INCLUDEPATH += $$PWD/src/ORM_SQLite \
               $$PWD/src/crypto \
               $$PWD/Controller \
               $$PWD/Model \
               $$PWD/DB



SOURCES += \
    $$PWD/3rd/sqlite3/sqlite3.c \
    Controller/support.cpp \
    Controller/vaultcontroller.cpp \
    DB/PathDB.cpp \
    DB/Tbl_Bucket.cpp \
    DB/Tbl_Del.cpp \
    DB/Tbl_Node.cpp \
    DB/Tbl_Nodedata.cpp \
    DB/Tbl_PathVault.cpp \
    DB/Tbl_Vaultconfig.cpp \
    DB/Tbl_Xattr.cpp \
    DB/VaultDB.cpp \
    Model/model.cpp \
    main.cpp \
    src/ORM_SQLite/ezsql.cpp \
    src/crypto/aes_256_gcm.cpp \
    src/crypto/hkdf.cpp \
    src/crypto/scrypt.cpp

HEADERS += \
    $$PWD/3rd/sqlite3/sqlite3.h \
    Controller/support.h \
    Controller/vaultcontroller.h \
    DB/PathDB.h \
    DB/Tbl_Bucket.h \
    DB/Tbl_Del.h \
    DB/Tbl_Node.h \
    DB/Tbl_Nodedata.h \
    DB/Tbl_PathVault.h \
    DB/Tbl_Vaultconfig.h \
    DB/Tbl_Xattr.h \
    DB/VaultDB.h \
    Model/model.h \
    logger.h \
    src/ORM_SQLite/ezsql.h \
    src/crypto/aes_256_gcm.h \
    src/crypto/hkdf.h \
    src/crypto/scrypt.h


RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
