#include <QDebug>
#include <QtWebSockets/QWebSocket>
#include <QFile>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>
#include "connection.h"
#include "Codes.h"

#define MyDebug() qDebug() << "Connection:"

Connection::Connection(const QUrl& url, QObject *parent) :
    QObject(parent),
    m_url(url)
{
    MyDebug() << "construct";
    qRegisterMetaType<QAbstractSocket::SocketState>("QAbstractSocket::SocketState");
}

Connection::~Connection()
{
    if(m_isConnected)
    {
        m_socket->close();
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
    m_socket = new QWebSocket("PreschoolSocket", QWebSocketProtocol::VersionLatest, this);
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
        {Protocol::LOGIN, login},
        {Protocol::PASSWORD, password}
    };

    sendMessage(obj);
}

void Connection::onTextMessageReceived(const QString &message)
{
    MyDebug() << Q_FUNC_INFO;
}

void Connection::onConnected()
{
    MyDebug() << Q_FUNC_INFO;
    setIsConnected(true);
}

void Connection::onDisconnected()
{
    MyDebug() << Q_FUNC_INFO;
    setIsConnected(false);

    QTimer::singleShot(5000, this, &Connection::tryConnect);
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
