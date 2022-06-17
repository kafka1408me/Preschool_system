QT += qml quick quickcontrols2 websockets

CONFIG += c++17

TARGET = PreschoolClient

SOURCES += \
    connection.cpp \
    main.cpp

RESOURCES += \
    qml.qrc

HEADERS += \
    connection.h \
    ../PreschoolServer/Codes.h

INCLUDEPATH += ../PreschoolServer
