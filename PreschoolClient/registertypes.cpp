#include <QGuiApplication>
#include <QQmlEngine>
#include <QAbstractSocket>
#include "Codes.h"

static void registerTypes()
{
    qRegisterMetaType<QAbstractSocket::SocketState>("QAbstractSocket::SocketState");

    qRegisterMetaType<User::UserRole>("User::UserRole");

    qmlRegisterUncreatableMetaObject(
                User::staticMetaObject,       // static meta object
                "preschool",             // import statement (can be any string)
                1, 0,                          // major and minor version of the import
                "UserRole",                       // name in QML (does not have to match C++ name)
                "Error: only enums"            // error in case someone tries to create a MyNamespace object
                );
}


Q_COREAPP_STARTUP_FUNCTION(registerTypes)
