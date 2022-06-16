#include <QDir>
#include <QCoreApplication>
#include <QSslCertificate>
#include <QSslConfiguration>
#include <QWebSocketServer>
#include <QSslKey>
#include <QSharedPointer>
#include "server.h"
#include "connectionhandler.h"
#include "databaseaccessor.h"

#define MyDebug() qDebug() << "Server:"

constexpr const auto SslDocName = "preschool";
constexpr int MaxPendingConnections = 100;

QString Server::ServerName = QString("PreschoolServer");

Server::Server(int port, QObject *parent):
    QObject(parent),
    m_port(port)
{
    MyDebug() << "construct";
}

void Server::start()
{
    QString applicationDirPath = qApp->applicationDirPath();
    // открываем файлы с сертификатами
    QString certFileName = QDir::toNativeSeparators(applicationDirPath + QString("/keys/") + SslDocName + ".pem");
    QFile certFile(certFileName);
    if (!certFile.exists())
    {
        MyDebug() << QString("CERTFILE '%1' NOT EXIST!!!!!!!!!!!").arg(certFileName);
        exit(-1);
    }

    QString keyFileName = QDir::toNativeSeparators(applicationDirPath + QString("/keys/") + SslDocName + ".key");
    QFile keyFile(keyFileName);
    if (!keyFile.exists())
    {
        MyDebug() << QString("KEYFILE '%1' NOT EXIST!!!!!!!!!!!").arg(keyFileName);
        exit(-2);
    }

    if (!certFile.open(QIODevice::ReadOnly))
    {
        MyDebug() << "CERTFILE can 't be opened";
        exit(-3);
    }
    if (!keyFile.open(QIODevice::ReadOnly))
    {
        MyDebug() << "KEYFILE can 't be opened";
        exit(-4);
    }

    QSslCertificate certificate(&certFile, QSsl::Pem);
    QSslKey sslKey(&keyFile, QSsl::Rsa, QSsl::Pem);
    certFile.close();
    keyFile.close();

    //ssl конфиг для сервера
    QSslConfiguration sslConfigServer;

    sslConfigServer.setPeerVerifyMode(QSslSocket::VerifyNone);
    sslConfigServer.setLocalCertificate(certificate);
    sslConfigServer.setPrivateKey(sslKey);
    sslConfigServer.setProtocol(QSsl::TlsV1_1OrLater);

    m_webSocketServer = new QWebSocketServer(ServerName,
                                           QWebSocketServer::SslMode::SecureMode,
                                           this);

    m_webSocketServer->setSslConfiguration(sslConfigServer);

    m_databaseAccessor = new DatabaseAccessor(this);
    m_databaseAccessor->start();

    this->startListen();
}

void Server::startListen()
{
    MyDebug() << Q_FUNC_INFO;

    if (m_webSocketServer->listen(QHostAddress::Any, m_port))
    {
        MyDebug() << "server listening on port" << m_port;
        connect(m_webSocketServer, &QWebSocketServer::newConnection,this, &Server::onNewConnection);

        m_webSocketServer->setMaxPendingConnections(MaxPendingConnections);
        MyDebug() << "max pending connections = " << m_webSocketServer->maxPendingConnections();
    }
    else
    {
        MyDebug() << "failed connecting to port " << m_port;
        MyDebug() << "error: " << m_webSocketServer->errorString();
        exit(-5);
    }
}

void Server::onNewConnection()
{
    MyDebug() << Q_FUNC_INFO;
    QWebSocket* pSocket = m_webSocketServer->nextPendingConnection();
    ConnectionHandler::ConnectionPtr connectionHandler = ConnectionHandler::createConnectionHandler(pSocket);
    connect(connectionHandler.data(), &ConnectionHandler::connectionClosed,
            this, &Server::onConnectionClosed);
    m_connections.push_back(std::move(connectionHandler));
}

void Server::onConnectionClosed(ConnectionHandler::ConnectionPtr connectionHandler)
{
    MyDebug() << Q_FUNC_INFO;
    m_connections.removeOne(connectionHandler);
}
