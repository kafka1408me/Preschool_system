#pragma once

#include <QObject>
#include "connection.h"

class ConnectionWrapper : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isConnected READ isConnected NOTIFY isConnectedChanged)
public:
    explicit ConnectionWrapper(const QUrl& url, QObject *parent = nullptr);

    ~ConnectionWrapper();

    void start();

    bool isConnected() const;

    Q_INVOKABLE void logIn(const QString& login, const QString& password);

    Q_INVOKABLE void logOut();

signals:
    void startConnecting();

    void isConnectedChanged(bool connected);

    void tryLogIn(const QString& login, const QString& password);

    void logInSuccess();

    void logInFailed();

    void tryLogOut();

private slots:
    void onConnectedChanged(bool connected);

private:
    QThread* m_connectionThread{nullptr};
    QScopedPointer<Connection> m_connection;
    bool m_isConnected{false};
};

