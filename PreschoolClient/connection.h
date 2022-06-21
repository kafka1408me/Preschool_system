#pragma once

#include <QObject>
#include <QUrl>
#include <QSslError>
#include <QJsonArray>
#include "Codes.h"

class QWebSocket;
class QTimer;

class Connection : public QObject
{
    Q_OBJECT
public:
    explicit Connection(const QUrl& url, QObject *parent = nullptr);

    ~Connection();

public slots:
    void close();

private:
    void setIsConnected(bool connected);

    void sendMessage(const QJsonObject& obj);

signals:
    void isConnectedChanged(bool connected);

    void logInSuccess();

    void logInFailed();

    void userNameReceived(const QString& userName);

    void userRoleReceived(User::UserRole userRole);

    void usersReceived(const QJsonArray& users);

    void childrenReceived(const QJsonArray& users);

    void showMessage(QString message);

    void testsReceived(QJsonArray tests);

public slots:
    void start();

    void logIn(const QString& login, const QString& password);

    void logOut();

    void getAllUsers();

    void getChildren();

    void getChildTeacher();

    void getChildrenParents();

    void createTest(const QString& testName, const QStringList& questions);

    void getTests();

    void uploadTest(UserIdType id, QJsonArray answers);

private slots:
    void onTextMessageReceived(const QString& message);

    void onConnected();

    void onDisconnected();

    void tryConnect();

    void onSslErrorsOccured(const QList<QSslError> &errors);

private:
    QWebSocket* m_socket{nullptr};
    QTimer* m_reconnectTimer{nullptr};
    QUrl m_url;
    bool m_isConnected{false};
};

