CONFIG += c++17

QT += core sql network websockets

TARGET = PreschoolServer

HEADERS += \
    application.h \
    connectionhandler.h \
    databaseaccessor.h \
    server.h \
    Codes.h

SOURCES += \
    application.cpp \
    connectionhandler.cpp \
    databaseaccessor.cpp \
    main.cpp \
    server.cpp

CONFIG(debug, debug|release){
  DESTDIR = $${PWD}/$${TARGET}_debug
}else{
  DESTDIR = $${PWD}/$${TARGET}_release
}

defineTest(copyDir) {
    CPDIR = $$1
    DDIR = $$2

    win32: {
        CPDIR ~= s,/,\\,g
        DDIR ~= s,/,\\,g
    }

    exists($$DDIR) {
    }
    else {
        QMAKE_PRE_LINK += $(MKDIR) $$DDIR $$escape_expand(\\n\\t)
    }

    QMAKE_PRE_LINK += $$QMAKE_COPY_DIR $$quote($$CPDIR) $$quote($$DDIR) $$escape_expand(\\n\\t)

    export(QMAKE_PRE_LINK)
}

defineTest(copyTo) {
    files = $$1
    DDIR = $$2

    win32:DDIR ~= s,/,\\,g

    exists($$DDIR) {
    }
    else {
        QMAKE_PRE_LINK += $(MKDIR) $$DDIR $$escape_expand(\\n\\t)
    }

    for(FILE, files) {
        # Replace slashes in paths with backslashes for Windows
        win32:FILE ~= s,/,\\,g

        QMAKE_PRE_LINK += $$QMAKE_COPY $$quote($$FILE) $$quote($$DDIR) $$escape_expand(\\n\\t)
    }

    export(QMAKE_PRE_LINK)
}

copyDir($$PWD/keys, $$DESTDIR/keys)
copyTo($$PWD/libpq.dll, $$DESTDIR)
copyTo($$PWD/config.ini, $$DESTDIR)
