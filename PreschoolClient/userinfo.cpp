#include <QStandardPaths>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QSaveFile>
#include <QDir>
#include "userinfo.h"

const QString UserName = QStringLiteral("userName");
const QString UserRole = QStringLiteral("userRole");

#define UserInfoDebug() qDebug() << "UserInfo:"

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
    getData();
}

UserInfo *UserInfo::getInstance()
{
    static UserInfo userInfo;
    return &userInfo;
}

void UserInfo::clearData()
{
    UserInfoDebug() << Q_FUNC_INFO;

    m_userName = "";

    QFile f(getDataFileName());
    f.remove();
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

QString UserInfo::getUserName() const
{
    return m_userName;
}

User::UserRole UserInfo::getUserRole() const
{
    return m_userRole;
}

void UserInfo::save()
{
    UserInfoDebug() << Q_FUNC_INFO;

    QSaveFile f(getDataFileName());
    if(!f.open(QIODevice::WriteOnly))
    {
        UserInfoDebug() << "open file FAILED" << getDataFileName();
        return;
    }

    QJsonObject obj;
    obj.insert(UserName, m_userName);
    obj.insert(UserRole, int(m_userRole));

    QJsonDocument doc(obj);
    if(!f.write(doc.toJson()))
    {
        UserInfoDebug() << "write file FAILED";
    }

    if(!f.commit())
    {
        UserInfoDebug() << "commit FAILED";
        return;
    }


    UserInfoDebug() << "data was saved SUCCESS";
}

void UserInfo::getData()
{
    QJsonObject obj = getDataFromFile();
    UserInfoDebug() << "data from file" << obj;
    if(obj.isEmpty())
    {
        UserInfoDebug() << "data obj is empty";
        return;
    }

    m_userName = obj.value(UserName).toString();
    m_userRole = User::UserRole(obj.value(UserRole).toInt());
}
