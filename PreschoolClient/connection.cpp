#include <QDebug>
#include <QtWebSockets/QWebSocket>
#include <QFile>
#include <QTimer>
#include "connection.h"
#include "Codes.h"

#define MyDebug() qDebug() << "Connection:"

Connection::Connection(const QUrl& url, QObject *parent) :
    QObject(parent),
    m_url(url)
{
    MyDebug() << "construct";
}

Connection::~Connection()
{
    if(m_isConnected)
    {
        m_socket->close();
    }
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

void Connection::onTextMessageReceived(const QString &message)
{
    MyDebug() << Q_FUNC_INFO;
}

void Connection::onConnected()
{
    MyDebug() << Q_FUNC_INFO;
    m_isConnected = true;
}

void Connection::onDisconnected()
{
    MyDebug() << Q_FUNC_INFO;
    m_isConnected = false;

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
