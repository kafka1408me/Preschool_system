#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQuickControls2/QQuickStyle>
#include <QQmlContext>
#include "connectionwrapper.h"
#include "Codes.h"
#include "userinfo.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    ConnectionWrapper connection{QUrl(QString("wss://192.168.1.64:%1").arg(DefaultPort))};
    connection.start();

    UserInfo::getInstance()->setUserRole(UserRole::Admin);

    QQmlApplicationEngine engine;
    auto context = engine.rootContext();
    context->setContextProperty("connection", &connection);
    context->setContextProperty("userInfo", UserInfo::getInstance());

    engine.load(QUrl(QLatin1String("qrc:/qml/main.qml")));

    if (engine.rootObjects().isEmpty())
    {
        return -1;
    }

    return app.exec();
}
