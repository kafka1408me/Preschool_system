#pragma once

#include <QObject>
#include "connectionhandler.h"

class QWebSocketServer;
class ConnectionHandler;
class DatabaseAccessor;

class Server: public QObject
{
    Q_OBJECT
public:

    explicit Server(int port, QObject* parent = nullptr);

    void start();

private:
    void startListen();

private slots:
    void onNewConnection();

    void onConnectionClosed(ConnectionHandler::ConnectionPtr connectionHandler);

public:
    static QString ServerName;

private:
    int m_port;
    QWebSocketServer* m_webSocketServer{nullptr};
    DatabaseAccessor* m_databaseAccessor{nullptr};
    QList<ConnectionHandler::ConnectionPtr> m_connections;
};

