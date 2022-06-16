#pragma once

#include <QObject>
#include <QEnableSharedFromThis>
#include "Codes.h"

class QWebSocket;


class ConnectionHandler : public QObject, public QEnableSharedFromThis<ConnectionHandler>
{
    Q_OBJECT
    Q_DISABLE_COPY(ConnectionHandler)
public:
    using ConnectionPtr = QSharedPointer<ConnectionHandler>;

    static ConnectionPtr createConnectionHandler(QWebSocket* socket);

    void sendMessage(const QJsonObject& obj);

    void setUserId(UserIdType id);

    void setUserName(const QString& name);

    void setUserRole(UserRole role);

private:
    explicit ConnectionHandler(QWebSocket* socket, QObject *parent = nullptr);

signals:
    void connectionClosed(ConnectionPtr connectionHandler);

    void requestDatabase(const QJsonObject& obj, ConnectionPtr connectionHadler);

    void sendTextMessage(const QString& message);

private slots:
    void onMessageReceived(const QString& message);

    void onSocketDisconnected();

private:
    QWebSocket* m_socket;
    UserIdType m_userId{DefaultUserId};
    QString m_userName;
    UserRole m_userRole{UserRole::Parent};
};

