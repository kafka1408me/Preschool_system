#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QJsonObject>
#include "connectionhandler.h"

class DatabaseAccessor: public QObject
{
    Q_OBJECT
public:
    DatabaseAccessor(QObject* parent = nullptr);

    ~DatabaseAccessor();

public slots:
    void start();

    void onRequest(const QJsonObject& obj, ConnectionHandler::ConnectionPtr connectionHandler);

private:
    bool createUser(const QString& userLogin,
                    const QString& userPassword,
                    const QString& userName,
                    User::UserRole userRole);

private:
    QSqlDatabase db;
};


