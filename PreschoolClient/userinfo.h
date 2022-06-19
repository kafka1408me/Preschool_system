#pragma once

#include <QObject>
#include "usersmodel.h"
#include "usersproxymodel.h"
#include "Codes.h"


class ChildrenModel;

class UserInfo: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString userName READ getUserName CONSTANT)
    Q_PROPERTY(User::UserRole userRole READ getUserRole CONSTANT)

    Q_PROPERTY(QAbstractItemModel* usersModel READ getUsersProxyModel CONSTANT)
    Q_PROPERTY(QAbstractItemModel* childrenModel READ getChildrenModel CONSTANT)

    Q_DISABLE_COPY(UserInfo)
public:
    static UserInfo *getInstance();

    Q_INVOKABLE void clearData();

    Q_INVOKABLE void setUsersRoleForModel(UserRole usersRole);

    QString getUserName() const;

    User::UserRole getUserRole() const;

    QAbstractItemModel *getUsersProxyModel() const;

    QAbstractItemModel *getChildrenModel() const;

public slots:
    void setUserName(const QString& name);

    void setUserRole(const User::UserRole role);

    void setUsers(const QJsonArray& users);

    void setChildren(const QJsonArray& users);

private:
    UserInfo();

    void save();

    void getData();

private:
    QString m_userName;
    User::UserRole m_userRole{User::UserRole::Parent};
    QScopedPointer<UsersModel> m_usersModel;
    QScopedPointer<UsersProxyModel> m_proxyUsersModel;
    QScopedPointer<ChildrenModel> m_childrenModel;
};

