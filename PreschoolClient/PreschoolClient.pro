QT += qml quick quickcontrols2 websockets

CONFIG += c++17

TARGET = PreschoolClient

SOURCES += \
    connection.cpp \
    connectionwrapper.cpp \
    main.cpp \
    registertypes.cpp \
    userinfo.cpp \
    usersmodel.cpp \
    usersproxymodel.cpp

RESOURCES += \
    qml.qrc

HEADERS += \
    connection.h \
    ../PreschoolServer/Codes.h \
    connectionwrapper.h \
    userinfo.h \
    usersmodel.h \
    usersproxymodel.h

INCLUDEPATH += ../PreschoolServer
