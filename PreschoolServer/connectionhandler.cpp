#include <QWebSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include "connectionhandler.h"
#include "Codes.h"

#define MyDebug() qDebug() << "ConnectionHandler:" << m_socket

ConnectionHandler::ConnectionHandler(QWebSocket* socket, QObject *parent) :
    QObject(parent),
    m_socket(socket)
{
    MyDebug() << "construct";

    connect(m_socket, &QWebSocket::textMessageReceived,
            this, &ConnectionHandler::onMessageReceived);
    connect(m_socket, &QWebSocket::disconnected,
            this, &ConnectionHandler::onSocketDisconnected);
}

void ConnectionHandler::onMessageReceived(const QString &message)
{
    MyDebug() << Q_FUNC_INFO;
    MyDebug() << "received message" << message;

    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());

    if(doc.isNull())
    {
        MyDebug() << "doc is null";
        return;
    }
    if(!doc.isObject())
    {
        MyDebug() << "doc is not object";
        return;
    }

    QJsonObject obj = doc.object();

    int type = obj.value(Protocol::MESSAGE_TYPE).toInt(-1);

    switch (type)
    {
    case Protocol::Codes::Authorization:
    {

        break;
    }
    default:
    {
        MyDebug() << "undefined message type" << type;
        break;
    }
    }
}

void ConnectionHandler::onSocketDisconnected()
{
    emit connectionClosed(this->sharedFromThis());
}
