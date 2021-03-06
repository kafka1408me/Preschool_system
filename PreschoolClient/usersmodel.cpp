#include <QJsonArray>
#include <QJsonObject>
#include "usersmodel.h"

PreschoolUser::PreschoolUser(UserIdType id, const QString &name, const QString &login, User::UserRole role):
    m_id(id),
    m_name(name),
    m_login(login),
    m_role(role)
{
}

UserIdType PreschoolUser::getUserId() const
{
    return m_id;
}

QString PreschoolUser::getUserName() const
{
    return m_name;
}

QString PreschoolUser::getUserLogin() const
{
    return m_login;
}

User::UserRole PreschoolUser::getUserRole() const
{
    return m_role;
}


UsersModel::UsersModel(QObject *parent) : QAbstractListModel(parent)
{

}

int UsersModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_users.size();
}

QVariant UsersModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_users.size())
    {
        return {};
    }

    const PreschoolUser& user = m_users.at(index.row());

    switch (role)
    {
    case UserID:
    {
        return user.getUserId();
    }
    case UserLogin:
    {
        return user.getUserLogin();
    }
    case UserName:
    {
        return user.getUserName();
    }
    case UserRole:
    {
        return user.getUserRole();
    }
    default:
    {
        break;
    }
    }
    return {};
}

QHash<int, QByteArray> UsersModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[UserID] = "id";
    roles[UserLogin] = "login";
    roles[UserName] = "name";
    roles[UserRole] = "role";

    return roles;
}

void UsersModel::setUsers(const QJsonArray &array)
{
    beginResetModel();
    m_users.clear();

    m_users.reserve(array.size());

    QJsonObject userObj;
    for(const QJsonValue& jsonVal: array)
    {
        userObj = jsonVal.toObject();

        QString name = userObj[Protocol::USER_NAME].toString();
        QString login = userObj[Protocol::USER_LOGIN].toString();
        UserIdType id = userObj[Protocol::USER_ID].toVariant().value<UserIdType>();
        User::UserRole role = User::UserRole(userObj[Protocol::USER_ROLE].toInt());

        m_users.push_back(PreschoolUser{id, name, login, role});
    }

    qDebug() << "users count" << m_users.size();

    endResetModel();
}

const PreschoolUser *UsersModel::getUserById(UserIdType id)
{
    auto it = std::find_if(m_users.begin(), m_users.end(), [id](const PreschoolUser& user) {
        return user.getUserId() == id;
    });
    if(it != m_users.end())
    {
        return &(*it);
    }
    return nullptr;
}

void UsersModel::clear()
{
    beginResetModel();
    m_users.clear();
    endResetModel();
}

QVariantMap UsersModel::getItemMap(int _index)
{
    QVariantMap varMap;
    if(_index < 0 || _index >= m_users.size())
    {
        return varMap;
    }

    const auto roles = roleNames();

    for(auto it = roles.cbegin(), end = roles.cend(); it != end; ++it)
    {
        varMap[it.value()] = data(index(_index, 0), it.key());
    }

    return varMap;
}

