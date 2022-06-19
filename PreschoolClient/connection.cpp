#include <QDebug>
#include <QtWebSockets/QWebSocket>
#include <QFile>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTimer>
#include "connection.h"
#include "Codes.h"
#include "userinfo.h"

#define MyDebug() qDebug() << "Connection:"

Connection::Connection(const QUrl& url, QObject *parent) :
    QObject(parent),
    m_url(url)
{
    MyDebug() << "construct";
    connect(this, &Connection::userNameReceived, UserInfo::getInstance(), &UserInfo::setUserName);
    connect(this, &Connection::userRoleReceived, UserInfo::getInstance(), &UserInfo::setUserRole);
    connect(this, &Connection::usersReceived, UserInfo::getInstance(), &UserInfo::setUsers);
    connect(this, &Connection::childrenReceived, UserInfo::getInstance(), &UserInfo::setChildren);
}

Connection::~Connection()
{
    close();
}

void Connection::close()
{
    if(m_isConnected)
    {
        m_socket->close();
        setIsConnected(false);
    }
    if(m_socket)
    {
        m_socket->deleteLater();
        m_socket = nullptr;
    }
    if(m_reconnectTimer)
    {
        m_reconnectTimer->deleteLater();
        m_reconnectTimer = nullptr;
    }
}


void Connection::setIsConnected(bool connected)
{
    if(m_isConnected == connected)
    {
        return;
    }

    m_isConnected = connected;
    emit isConnectedChanged(m_isConnected);
}

void Connection::sendMessage(const QJsonObject &obj)
{
    MyDebug() << "send message" << obj;

    QJsonDocument doc{obj};
    QString message = doc.toJson(QJsonDocument::Compact);

    m_socket->sendTextMessage(message);
}

void Connection::start()
{
    MyDebug() << Q_FUNC_INFO;
    m_socket = new QWebSocket("PreschoolSocket", QWebSocketProtocol::VersionLatest);
    connect(m_socket, &QWebSocket::connected, this, &Connection::onConnected);
    connect(m_socket, &QWebSocket::disconnected, this, &Connection::onDisconnected);
    connect(m_socket, &QWebSocket::textMessageReceived, this, &Connection::onTextMessageReceived);
    connect(m_socket, &QWebSocket::sslErrors, this, &Connection::onSslErrorsOccured);

    QSslConfiguration sslConfig = m_socket->sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyPeer);

    QFile f{":/key/ca_cert.pem"};
    QByteArray certData;
    if(!f.open(QIODevice::ReadOnly))
    {
        MyDebug() << "cert not found !";
        exit(-900);
    }
    certData = f.readAll();
    f.close();

    QSslCertificate certSsl{certData};
    sslConfig.addCaCertificate(certSsl);

    m_socket->setSslConfiguration(sslConfig);

    tryConnect();
}

void Connection::logIn(const QString &login, const QString &password)
{
    MyDebug() << Q_FUNC_INFO;

    QJsonObject obj {
        {Protocol::MESSAGE_TYPE, Protocol::Codes::Authorization},
        {Protocol::USER_LOGIN, login},
        {Protocol::PASSWORD, password}
    };

    sendMessage(obj);
}

void Connection::logOut()
{
    MyDebug() << Q_FUNC_INFO;

    QJsonObject obj {
        {Protocol::MESSAGE_TYPE, Protocol::Codes::LogOut},
    };

    sendMessage(obj);
}

void Connection::getAllUsers()
{
    MyDebug() << Q_FUNC_INFO;

    QJsonObject obj {
        {Protocol::MESSAGE_TYPE, Protocol::Codes::GetAllUsers},
    };

    sendMessage(obj);
}

void Connection::getChildren()
{
    MyDebug() << Q_FUNC_INFO;

    QJsonObject obj {
        {Protocol::MESSAGE_TYPE, Protocol::Codes::GetChildren},
    };

    sendMessage(obj);
}

void Connection::getChildTeacher()
{
    MyDebug() << Q_FUNC_INFO;

    QJsonObject obj {
        {Protocol::MESSAGE_TYPE, Protocol::Codes::GetChildTeacher},
    };

    sendMessage(obj);
}

void Connection::onTextMessageReceived(const QString &message)
{
    MyDebug() << "message received" << message;

    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());

    if(doc.isNull())
    {
        MyDebug() << "doc is null";
        return;
    }
    if(!doc.isObject())
    {
        MyDebug() << "doc is not object!";
        return;
    }

    QJsonObject obj = doc.object();

    int type = obj.value(Protocol::MESSAGE_TYPE).toInt();
    int result = obj.value(Protocol::RESULT).toInt(Protocol::RESULT_FAIL);

    switch (type)
    {
    case Protocol::Codes::Authorization:
    {
        if(result == Protocol::RESULT_SUCCESS)
        {
            QString userName = obj.value(Protocol::USER_NAME).toString();
            User::UserRole userRole = User::UserRole(obj.value(Protocol::USER_ROLE).toInt());

            emit userNameReceived(userName);
            emit userRoleReceived(userRole);

            emit logInSuccess();
        }
        else
        {
            emit logInFailed();
        }
        break;
    }
    case Protocol::Codes::GetAllUsers:
    {
        if(result == Protocol::RESULT_SUCCESS)
        {
            QJsonArray users = obj.value(Protocol::USERS).toArray();
            emit usersReceived(users);
        }
        break;
    }
    case Protocol::Codes::GetChildren:
    {
        if(result == Protocol::RESULT_SUCCESS)
        {
            QJsonArray children = obj.value(Protocol::CHILDREN).toArray();
            emit childrenReceived(children);
        }
        break;
    }
    case Protocol::Codes::GetChildTeacher:
    {
        if(result == Protocol::RESULT_SUCCESS)
        {
            QJsonArray users = obj.value(Protocol::USERS).toArray();
            emit usersReceived(users);
        }
        break;
    }
    default:
    {
        MyDebug() << "undefined message type" << type;
        break;
    }
    }
}

void Connection::onConnected()
{
    MyDebug() << Q_FUNC_INFO;
    if(m_reconnectTimer)
    {
        m_reconnectTimer->stop();
    }
    setIsConnected(true);
}

void Connection::onDisconnected()
{
    MyDebug() << Q_FUNC_INFO;
    setIsConnected(false);

    if(!m_reconnectTimer)
    {
        m_reconnectTimer = new QTimer;
        m_reconnectTimer->setInterval(5000);
        m_reconnectTimer->setSingleShot(true);
        connect(m_reconnectTimer, &QTimer::timeout, this, &Connection::tryConnect);
    }
    m_reconnectTimer->start();
}

void Connection::tryConnect()
{
    MyDebug() << "try connect to url" << m_url;
    m_socket->open(m_url);
}

void Connection::onSslErrorsOccured(const QList<QSslError> &errors)
{
    MyDebug() << Q_FUNC_INFO;
    for(const QSslError& error: errors)
    {
        MyDebug() << error;
    }
}
