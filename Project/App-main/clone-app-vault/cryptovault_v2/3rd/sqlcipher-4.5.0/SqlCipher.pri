
linux {
    INCLUDEPATH += $$PWD/dist_linux/include
    LIBS += -L$$PWD/dist_linux/lib -lsqlcipher 
}

macx {
    INCLUDEPATH += $$PWD/dist_macx/include
    LIBS += -L$$PWD/dist_macx/lib -lsqlcipher
}

win32-g++ {
    INCLUDEPATH += $$PWD/dist_mingw/include
    LIBS += -L$$PWD/dist_mingw/lib -lsqlcipher \
            -L$$PWD/../openssl_1_1_1w/dist_mingw/lib -lssl -lcrypto

}

OpenSSL_DIR = $$PWD/../../3rd/openssl_1_1_1w
include($$OpenSSL_DIR/OpenSSL.pri)
