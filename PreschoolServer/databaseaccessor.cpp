#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QRandomGenerator>
#include <QCryptographicHash>
#include "databaseaccessor.h"
#include "application.h"
#include "Codes.h"

#define MyDebug() qDebug() << "DatabaseAccessor:"
#define MyWarning() qWarning() << "DatabaseAccessor:"

static constexpr char myStr[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
constexpr auto randStrSize = sizeof (myStr)/sizeof (char) - 1;

constexpr int saltLength = 64;

static auto randomGenerator = QRandomGenerator::system();

QString createRandomStringAdditional(const int length, const char* str, const int strLength)
{
    char randArray[length + 1];
    randArray[length] = '\0';

    for(int i = 0; i < length; ++i)
    {
        int indx = randomGenerator->generate() % strLength;
        randArray[i] = str[indx];
    }

    return QString(randArray);
}

QString createRandomString(int length)
{
    return createRandomStringAdditional(length, myStr, randStrSize);
}

QByteArray createHashPwd(const QString& pwd, const QString& salt)
{
    return QCryptographicHash::hash((pwd+salt).toUtf8(), QCryptographicHash::Algorithm::Sha512);
}

DatabaseAccessor::DatabaseAccessor(QObject *parent):
    QObject(parent)
{
    MyDebug() << "construct";
}

DatabaseAccessor::~DatabaseAccessor()
{
    db.close();
}

void DatabaseAccessor::start()
{
    MyDebug() << Q_FUNC_INFO;

    db = QSqlDatabase::addDatabase("QPSQL");

    QString dbHost = MyApp->getDatabaseHost();
    QString dbName = MyApp->getDatabaseName();
    QString dbUser = MyApp->getDatabaseUser();
    QString dbPassword = MyApp->getDatabasePassword();

    db.setHostName(dbHost);
    db.setDatabaseName(dbName);
    db.setUserName(dbUser);
    db.setPassword(dbPassword);
    if (db.open())
    {
        MyDebug() << "SUCCESS connection to database";
    }
    else
    {
        MyDebug() << "FAILURE connection to database: " << db.lastError().text();
        exit(-100);
    }

    createUser("admin", "admin", "Татьяна Алексеевна", User::UserRole::Admin);
    createUser("parent_0", "parent", "Алиса Хромова Петровна", User::UserRole::Parent);
    createUser("teacher_0", "teacher", "Виктория Озерова Васильевна", User::UserRole::Teacher);
//    createChild("Алексей Хромов Иванович", 5, 10, 11);
}

void DatabaseAccessor::onRequest(const QJsonObject &obj, ConnectionHandler::ConnectionPtr connectionHandler)
{
    using namespace Protocol;

    MyDebug() << Q_FUNC_INFO << obj;

    MyDebug() << Q_FUNC_INFO << obj;

    const int type = obj.value(MESSAGE_TYPE).toInt();

    switch (type)
    {
    case Codes::Authorization:
    {
        QString login = obj.value(LOGIN).toString();
        QString password = obj.value(PASSWORD).toString();

        QSqlQuery query(db);

        query.prepare(QStringLiteral("SELECT user_id, user_name, user_role, user_pwd, salt FROM users "
                                     "WHERE user_login=:user_login"));
        query.bindValue(QStringLiteral(":user_login"), login);

        QJsonObject responseObj;
        responseObj.insert(MESSAGE_TYPE, Codes::Authorization);

        query.exec();
        if(query.first())
        {
            QByteArray hash = query.value("user_pwd").toByteArray();
            QString salt = query.value("salt").toString();

            if(createHashPwd(password, salt) == hash)
            {
                UserIdType userId = query.value("user_id").value<UserIdType>();
                QString userName = query.value("user_name").toString();
                User::UserRole userRole = User::UserRole(query.value("user_role").toInt());

                connectionHandler->setUserId(userId);
                connectionHandler->setUserRole(userRole);
                connectionHandler->setUserName(userName);

                responseObj.insert(RESULT, RESULT_SUCCESS);
                responseObj.insert(USER_NAME, userName);
                responseObj.insert(USER_ROLE, userRole);
            }
            else
            {
                MyDebug() << "login failed";
                responseObj.insert(RESULT, RESULT_FAIL);
            }
        }
        else
        {
            MyWarning() << "request failed" << query.lastError().text();
            responseObj.insert(RESULT, RESULT_FAIL);
        }

        connectionHandler->sendMessage(responseObj);

        break;
    }
    default:
    {
        MyWarning() << "undefined message type" << type;
        break;
    }
    }
}

bool DatabaseAccessor::createUser(const QString &userLogin, const QString &userPassword, const QString &userName, User::UserRole userRole)
{
    MyDebug() << Q_FUNC_INFO;

    QString salt = createRandomString(saltLength);
    auto pwdHash = createHashPwd(userPassword, salt);

    QSqlQuery query(db);

    query.prepare(QStringLiteral("INSERT INTO users (user_name, user_role, salt, user_pwd, user_login) "
                                 "VALUES(:user_name, :user_role, :salt, :user_pwd, :user_login)"));
    query.bindValue(":user_name", userName);
    query.bindValue(":user_role", int(userRole));
    query.bindValue(":salt", salt);
    query.bindValue(":user_pwd", pwdHash);
    query.bindValue(":user_login", userLogin);

    if(query.exec())
    {
        MyDebug() << "create user SUCCESS";
        return true;
    }

    MyDebug() << "create user FAILED" << query.lastError().text();
    return false;
}

bool DatabaseAccessor::createChild(const QString &name, quint8 age, UserIdType parentId, UserIdType teacherId)
{
    MyDebug() << Q_FUNC_INFO;

    QSqlQuery query(db);

    query.prepare(QStringLiteral("INSERT INTO children (name, age, parent_id, teacher_id) "
                                 "VALUES(:name, :age, :parent_id, :teacher_id)"));
    query.bindValue(":name", name);
    query.bindValue(":age", age);
    query.bindValue(":parent_id", parentId);
    query.bindValue(":teacher_id", teacherId);

    if(query.exec())
    {
        MyDebug() << "create child SUCCESS";
        return true;
    }

    MyDebug() << "create child FAILED" << query.lastError().text();
    return false;
}

QJsonArray DatabaseAccessor::getAllParents()
{
    return getAllUsersByRole(User::UserRole::Parent);
}

QJsonArray DatabaseAccessor::getAllTeachers()
{
    return getAllUsersByRole(User::UserRole::Teacher);
}

QJsonArray DatabaseAccessor::getAllUsersByRole(User::UserRole role)
{
    QSqlQuery query(db);

    QJsonArray users;

    query.prepare(QStringLiteral("SELECT user_id, user_name, user_login FROM users WHERE user_role=:user_role"));
    query.bindValue(":user_role", role);

    if(query.exec())
    {
        QJsonObject user;

        while (query.next())
        {
            user[Protocol::USER_NAME] = query.value("user_name").toString();
            user[Protocol::LOGIN] = query.value("user_login").toString();
            user[Protocol::USER_ID] = query.value("user_id").toString();

            users.push_back(user);
        }
    }
    else
    {
        MyDebug() << "get users byr role" << role << "FAILED";
    }
    return users;
}

QJsonArray DatabaseAccessor::getChildren(bool forParent, UserIdType id)
{
    QSqlQuery query(db);
    QJsonArray children;

    QString queryText = "SELECT id, name, age, parent_id, teacher_id FROM children";
    if(id != DefaultUserId)
    {
        if(forParent)
        {
            queryText += " WHERE parent_id=:id";
        }
        else
        {
            queryText += " WHERE teacher_id=:id";
        }
        query.prepare(queryText);
        query.bindValue(":id", id);
    }
    else
    {
        query.prepare(queryText);
    }

    if(query.exec())
    {
        QJsonObject child;

        while (query.next())
        {
            child[Protocol::CHILD_NAME] = query.value("name").toString();
            child[Protocol::CHILD_AGE] = query.value("age").toString();
            child[Protocol::CHILD_ID] = query.value("id").toString();
            child[Protocol::CHILD_PARENT_ID] = qint64(query.value("parent_id").value<UserIdType>());
            child[Protocol::CHILD_TEACHER_ID] = qint64(query.value("teacher_id").value<UserIdType>());

            children.push_back(child);
        }
    }
    else
    {
        MyDebug() << "get all children FAILED";
    }
    return children;
}

QJsonArray DatabaseAccessor::getAllChildren()
{
    return getChildren();
}


QJsonArray DatabaseAccessor::getChildrenForTeacher(UserIdType id)
{
    return getChildren(false, id);
}

QJsonArray DatabaseAccessor::getChildrenForParent(UserIdType id)
{
    return getChildren(true, id);
}
