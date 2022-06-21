QT += qml quick quickcontrols2 websockets

CONFIG += c++17

TARGET = PreschoolClient

SOURCES += \
    childrenmodel.cpp \
    connection.cpp \
    connectionwrapper.cpp \
    main.cpp \
    proxychildrenmodel.cpp \
    registertypes.cpp \
    testsmodel.cpp \
    userinfo.cpp \
    usersmodel.cpp \
    usersproxymodel.cpp

RESOURCES += \
    qml.qrc

HEADERS += \
    childrenmodel.h \
    connection.h \
    ../PreschoolServer/Codes.h \
    connectionwrapper.h \
    proxychildrenmodel.h \
    testsmodel.h \
    userinfo.h \
    usersmodel.h \
    usersproxymodel.h

INCLUDEPATH += ../PreschoolServer

DISTFILES += \
    CreateTestPage.qml
