#pragma once

#include <QObject>
#include "Codes.h"

class UserInfo: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString userName READ getUserName)
    Q_PROPERTY(User::UserRole userRole READ getUserRole)

    Q_DISABLE_COPY(UserInfo)
public:
    static UserInfo *getInstance();

    Q_INVOKABLE void clearData();

    QString getUserName() const;

    User::UserRole getUserRole() const;

public slots:
    void setUserName(const QString& name);

    void setUserRole(const User::UserRole role);

private:
    UserInfo();

    void save();

    void getData();

private:
    QString m_userName;
    User::UserRole m_userRole{User::UserRole::Parent};
};

