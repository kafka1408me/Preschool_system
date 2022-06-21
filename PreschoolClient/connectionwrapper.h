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

    Q_INVOKABLE void getAllUsers();

    Q_INVOKABLE void getAllChildren();

    Q_INVOKABLE void getChildren();

    Q_INVOKABLE void getChildTeacher();

    Q_INVOKABLE void getChildrenParents();

    Q_INVOKABLE void createTest(QString testName, QVariantList questions);

    Q_INVOKABLE void getTests();

    Q_INVOKABLE void uploadTest(UserIdType id, QVariantList answers);

signals:
    void startConnecting();

    void isConnectedChanged(bool connected);

    void tryLogIn(const QString& login, const QString& password);

    void logInSuccess();

    void logInFailed();

    void tryLogOut();

    void tryGetAllUsers();

    void tryGetAllChildren();

    void tryGetChildren();

    void tryGetChildTeacher();

    void tryGetChildrenParents();

    void tryCreateTest(const QString& testName, const QStringList& questions);

    void showMessage(QString message);

    void tryGetTests();

    void tryUploadTest(UserIdType id, QJsonArray answers);

private slots:
    void onConnectedChanged(bool connected);

private:
    QThread* m_connectionThread{nullptr};
    QScopedPointer<Connection> m_connection;
    bool m_isConnected{false};
};

