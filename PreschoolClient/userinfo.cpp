#include <QStandardPaths>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QSaveFile>
#include <QDir>
#include "userinfo.h"

const QString userName = QStringLiteral("userName");
const QString userRole = QStringLiteral("userRole");

#define MyDebug() qDebug() << "UserInfo:"
#define MyWarning() qWarning() << "UserInfo:"

QString getWritebleLocation()
{
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
}

QString getDataFileName()
{
    return getWritebleLocation() + "/" "preschoolData.json";
}

QJsonObject getDataFromFile()
{
    QString dataFileName = getDataFileName();
    QFile f(dataFileName);
    if(f.exists())
    {
        if(f.open(QIODevice::ReadOnly))
        {
            QByteArray data = f.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(data);
            if(doc.isObject())
            {
                return doc.object();
            }
        }
    }
    else
    {
        QFileInfo fileInfo(dataFileName);
        QDir dir(fileInfo.dir());
        dir.mkpath(".");
    }
    return {};
}

UserInfo::UserInfo()
{
    m_usersModel.reset(new UsersModel);
    m_proxyUsersModel.reset(new UsersProxyModel(m_usersModel.data()));
    m_childrenModel.reset(new ChildrenModel(m_usersModel.data()));
    m_childrenProxyModel.reset(new ProxyChildrenModel(m_childrenModel.data()));
    m_testsModel.reset(new TestsModel);

    getData();
}

UserInfo::~UserInfo()
{

}

UserInfo *UserInfo::getInstance()
{
    static UserInfo userInfo;
    return &userInfo;
}

void UserInfo::clearData()
{
    MyDebug() << Q_FUNC_INFO;

    m_userName = "";

    m_usersModel->clear();
    m_childrenModel->clear();
    m_testsModel->clear();

    m_childrenProxyModel->setId(DefaultUserId);

    QFile f(getDataFileName());
    f.remove();
}

void UserInfo::setUsersRoleForModel(UserRole usersRole)
{
    m_proxyUsersModel->setUsersRole(usersRole);
}

QVariantMap UserInfo::getChild(int index)
{
    return m_childrenModel->getItemMap(index);
}

QVariantMap UserInfo::getTest(int index)
{
    return m_testsModel->getItemMap(index);
}

void UserInfo::setChildIdForShowing(UserIdType id)
{
    m_childrenProxyModel->setId(id);
}

QVariantList UserInfo::getAllUsers()
{
    QVariantList users;
    for(int i = 0; i < m_usersModel->rowCount(); ++i)
    {
        users.push_back(m_usersModel->getItemMap(i));
    }
    return users;
}

QVariantList UserInfo::getAllChildren()
{
    QVariantList children;
    for(int i = 0; i < m_childrenModel->rowCount(); ++i)
    {
        children.push_back(m_childrenModel->getItemMap(i));
    }
    return children;
}

void UserInfo::setUserName(const QString &name)
{
    m_userName = name;
    save();
}

void UserInfo::setUserRole(const User::UserRole role)
{
    m_userRole = role;

    save();
}

void UserInfo::setUsers(const QJsonArray &users)
{
    MyDebug() << Q_FUNC_INFO;

    m_usersModel->setUsers(users);
}

void UserInfo::setChildren(const QJsonArray &array)
{
    MyDebug() << Q_FUNC_INFO;
    m_childrenModel->setChildren(array);
}

void UserInfo::setTests(const QJsonArray &array)
{
    m_testsModel->setTests(array);
}

QString UserInfo::getUserName() const
{
    return m_userName;
}

User::UserRole UserInfo::getUserRole() const
{
    return m_userRole;
}

QAbstractItemModel *UserInfo::getUsersProxyModel() const
{
    return m_proxyUsersModel.data();
}

QAbstractItemModel *UserInfo::getChildrenModel() const
{
    return m_childrenProxyModel.data();
}

QAbstractItemModel *UserInfo::getTestsModel() const
{
    return m_testsModel.data();
}

void UserInfo::save()
{
    MyDebug() << Q_FUNC_INFO;

    QSaveFile f(getDataFileName());
    if(!f.open(QIODevice::WriteOnly))
    {
        MyDebug() << "open file FAILED" << getDataFileName();
        return;
    }

    QJsonObject obj;
    obj.insert(userName, m_userName);
    obj.insert(userRole, int(m_userRole));

    QJsonDocument doc(obj);
    if(!f.write(doc.toJson()))
    {
        MyDebug() << "write file FAILED";
    }

    if(!f.commit())
    {
        MyDebug() << "commit FAILED";
        return;
    }


    MyDebug() << "data was saved SUCCESS";
}

void UserInfo::getData()
{
    QJsonObject obj = getDataFromFile();
    MyDebug() << "data from file" << obj;
    if(obj.isEmpty())
    {
        MyDebug() << "data obj is empty";
        return;
    }

    m_userName = obj.value(userName).toString();
    m_userRole = User::UserRole(obj.value(userRole).toInt());
}
