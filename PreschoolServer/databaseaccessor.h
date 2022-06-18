#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QJsonObject>
#include <QJsonArray>
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

    bool createChild(const QString& name,
                     quint8 age,
                     UserIdType parentId,
                     UserIdType teacherId);

    QJsonArray getAllParents();

    QJsonArray getAllTeachers();

    QJsonArray getAllUsersByRole(User::UserRole role);

    QJsonArray getChildren(bool forParent = false, UserIdType id = DefaultUserId);

    QJsonArray getAllChildren();

    QJsonArray getChildrenForTeacher(UserIdType id);

    QJsonArray getChildrenForParent(UserIdType id);

private:
    QSqlDatabase db;
};


