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
    UserIdType getUserId() const;

    void setUserName(const QString& name);
    QString getUserName() const;

    void setUserRole(User::UserRole role);
    UserRole getUserRole() const;

    bool isUserAdmin() const;

    bool isUserParent() const;

    bool isUserTeacher() const;

private:
    explicit ConnectionHandler(QWebSocket* socket, QObject *parent = nullptr);

    void sendResultFailMessage(Protocol::Codes code);

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
    User::UserRole m_userRole{User::UserRole::Parent};
};

