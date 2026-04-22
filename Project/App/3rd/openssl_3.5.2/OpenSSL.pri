linux {
    INCLUDEPATH += $$PWD/dist_linux/include
    LIBS += -L$$PWD/dist_linux/lib -lssl -lcrypto \
            -ldl
}

macx {
    INCLUDEPATH += $$PWD/dist_macx/include
    LIBS += -L$$PWD/dist_macx/lib -lssl -lcrypto
}

win32-g++ {
    INCLUDEPATH += $$PWD/dist_mingw/include
    LIBS += -L$$PWD/dist_mingw/lib -lssl -lcrypto
}
