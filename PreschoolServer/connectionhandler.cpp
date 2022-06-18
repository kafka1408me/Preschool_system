#include <QWebSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include "connectionhandler.h"
#include "Codes.h"

#define MyDebug() qDebug() << "ConnectionHandler:" << m_socket

ConnectionHandler::ConnectionPtr ConnectionHandler::createConnectionHandler(QWebSocket *socket)
{
    return QSharedPointer<ConnectionHandler>{new ConnectionHandler(socket)};
}

void ConnectionHandler::sendMessage(const QJsonObject &obj)
{
    MyDebug() << Q_FUNC_INFO;
    QJsonDocument doc{obj};
    QString message = doc.toJson(QJsonDocument::Compact);

    emit sendTextMessage(message);
}

void ConnectionHandler::setUserId(UserIdType id)
{
    m_userId = id;
}

void ConnectionHandler::setUserName(const QString &name)
{
    m_userName = name;
}

void ConnectionHandler::setUserRole(User::UserRole role)
{
    m_userRole = role;
}

ConnectionHandler::ConnectionHandler(QWebSocket* socket, QObject *parent) :
    QObject(parent),
    m_socket(socket)
{
    MyDebug() << "construct";

    connect(m_socket, &QWebSocket::textMessageReceived,
            this, &ConnectionHandler::onMessageReceived,
            Qt::QueuedConnection);
    connect(m_socket, &QWebSocket::disconnected,
            this, &ConnectionHandler::onSocketDisconnected,
            Qt::QueuedConnection);
    connect(this, &ConnectionHandler::sendTextMessage,
            m_socket, &QWebSocket::sendTextMessage,
            Qt::QueuedConnection);
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
        if(m_userId == DefaultUserId)
        {
            emit requestDatabase(obj, sharedFromThis());
        }
        break;
    }
    case Protocol::Codes::LogOut:
    {
        m_userId = DefaultUserId;
        m_userName = "";
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
    emit connectionClosed(sharedFromThis());
}
