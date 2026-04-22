linux {
    INCLUDEPATH += $$PWD/dist_linux/include
    LIBS += -L$$PWD/dist_linux/lib -lssl -lcrypto
}

macx {
    INCLUDEPATH += $$PWD/dist_macx/include
    LIBS += -L$$PWD/dist_macx/lib -lssl -lcrypto
}

win32-g++ {
    INCLUDEPATH += $$PWD/dist_mingw/include
    LIBS += -L$$PWD/dist_mingw/lib -lssl -lcrypto -lws2_32 -lcrypt32 -lgdi32 -luser32
}
