#pragma once

#include <QAbstractListModel>
#include "Codes.h"

class PreschoolUser
{
public:
    PreschoolUser(UserIdType id, const QString& name, const QString& login, User::UserRole role);

    UserIdType getUserId() const;

    QString getUserName() const;

    QString getUserLogin() const;

    User::UserRole getUserRole() const;

private:
    UserIdType m_id;
    QString m_name;
    QString m_login;
    User::UserRole m_role;
};

class UsersModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles{
        UserID = Qt::UserRole + 1,
        UserName,
        UserRole,
        UserLogin
    };

    explicit UsersModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    void setUsers(const QJsonArray& array);

    const PreschoolUser *getUserById(UserIdType id);

    void clear();

     QVariantMap getItemMap(int _index);

private:
    QVector<PreschoolUser> m_users;
};

