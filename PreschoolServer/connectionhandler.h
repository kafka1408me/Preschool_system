#pragma once

#include <QObject>
#include <QEnableSharedFromThis>
#include "Codes.h"

class QWebSocket;

class ConnectionHandler : public QObject, public QEnableSharedFromThis<ConnectionHandler>
{
    Q_OBJECT
public:
    using ConnectionPtr = QSharedPointer<ConnectionHandler>;

    explicit ConnectionHandler(QWebSocket* socket, QObject *parent = nullptr);

signals:
    void connectionClosed(ConnectionPtr connectionHandler);

private slots:
    void onMessageReceived(const QString& message);

    void onSocketDisconnected();

private:
    QWebSocket* m_socket;
    UserIdType m_userId{DefaultUserId};
};

