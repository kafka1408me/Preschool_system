#pragma once

#include <QObject>
#include <QUrl>
#include <QSslError>

class QWebSocket;

class Connection : public QObject
{
    Q_OBJECT
public:
    explicit Connection(const QUrl& url, QObject *parent = nullptr);

    ~Connection();

    void start();

private slots:
    void onTextMessageReceived(const QString& message);

    void onConnected();

    void onDisconnected();

    void tryConnect();

    void onSslErrorsOccured(const QList<QSslError> &errors);

private:
    QWebSocket* m_socket;
    QUrl m_url;
    bool m_isConnected{false};
};

