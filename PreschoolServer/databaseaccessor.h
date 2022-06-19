#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QJsonObject>
#include <QJsonArray>
#include <optional>
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
                     Gender gender,
                     UserIdType parentId,
                     UserIdType teacherId);

    void addTrafficDays(UserIdType childId, int year, int month, const QList<int>& addingDays);

    std::optional<QList<int>> getTrafficDays(UserIdType childId, int year, int month);

    QJsonArray getAllParents();

    QJsonArray getAllTeachers();

    QJsonArray getAllUsersByRole(User::UserRole role);

    QJsonArray getAllUsers();

    QJsonArray getChildren(UserRole userRole = UserRole::Admin, UserIdType id = DefaultUserId);

    QJsonArray getAllChildren();

    QJsonArray getChildrenForTeacher(UserIdType id);

    QJsonArray getChildrenForParent(UserIdType id);

private:
    QSqlDatabase db;
};


