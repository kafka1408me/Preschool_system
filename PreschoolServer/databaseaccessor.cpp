#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QRandomGenerator>
#include <QCryptographicHash>
#include <QStringRef>
#include <QJsonDocument>
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

    createUser("admin", "admin", "Admin Admin", User::UserRole::Admin);
//    createUser("parent_0", "parent", "?????????? ?????????????? ????????????????", User::UserRole::Parent);
//    createUser("parent_1", "parent", "?????????? ?????????? ????????????????", User::UserRole::Parent);
//    createUser("parent_2", "parent", "?????????? ???????????? ????????????????????", User::UserRole::Parent);
//    createUser("teacher_0", "teacher", "???????????????? ?????????????? ????????????????????", User::UserRole::Teacher);
//    createUser("teacher_1", "teacher", "?????????? ???????????????? ????????????????????", User::UserRole::Teacher);
//    createChild("?????????????? ???????????? ????????????????",  5, Gender::Male, 10, 11);
//    createChild("?????????? ???????????? ??????????????????", 4, Gender::Female, 26, 11);
//    createChild("?????????????? ?????????? ????????????????", 6, Gender::Male, 160, 156);

    auto days1 = getTrafficDays(1, 2022, 6);
    addTrafficDays(1, 2022, 6, {16, 17});

//    qDebug() << getTestsForParent(10);
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
        QString login = obj.value(USER_LOGIN).toString();
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
    case Codes::GetAllUsers:
    {
        QJsonArray array = getAllUsers();

        QJsonObject responseObj;
        responseObj.insert(MESSAGE_TYPE, type);

        responseObj.insert(RESULT, RESULT_SUCCESS);
        responseObj.insert(USERS, array);

        connectionHandler->sendMessage(responseObj);

        break;
    }
    case Codes::GetChildren:
    {
        QJsonArray array;

        UserRole userRole = connectionHandler->getUserRole();
        UserIdType userId = connectionHandler->getUserId();

        array = getChildren(userRole, userId);

        QJsonObject responseObj;
        responseObj.insert(MESSAGE_TYPE, type);

        responseObj.insert(RESULT, RESULT_SUCCESS);
        responseObj.insert(CHILDREN, array);

        connectionHandler->sendMessage(responseObj);

        break;
    }
    case Codes::GetChildTeacher:
    {
        QSqlQuery query(db);

        query.prepare("SELECT user_id, user_name FROM users WHERE user_id IN (SELECT teacher_id FROM children WHERE parent_id=:id)");
        query.bindValue(":id", connectionHandler->getUserId());

        QJsonObject responseObj;
        responseObj.insert(Protocol::MESSAGE_TYPE, type);

        query.exec();
        if(query.first())
        {
            QJsonObject teacherObj;
            teacherObj.insert(Protocol::USER_ID, query.value("user_id").value<qint64>());
            teacherObj.insert(Protocol::USER_NAME, query.value("user_name").toString());

            responseObj.insert(Protocol::USERS, QJsonArray({teacherObj}));

            responseObj.insert(Protocol::RESULT, Protocol::RESULT_SUCCESS);
        }
        else
        {
            MyDebug() << "get child teacher FAILED" << query.lastError().text();
            responseObj.insert(Protocol::RESULT, Protocol::RESULT_FAIL);
        }

        connectionHandler->sendMessage(responseObj);

        break;
    }
    case Codes::GetChildrenParents:
    {
        QSqlQuery query(db);

        query.prepare("SELECT user_id, user_name FROM users WHERE user_id IN (SELECT parent_id FROM children WHERE teacher_id=:teacher_id)");
        query.bindValue(":teacher_id", connectionHandler->getUserId());

        QJsonObject responseObj;
        responseObj.insert(Protocol::MESSAGE_TYPE, type);

        if(query.exec())
        {
            QJsonArray parents;
            while (query.next())
            {
                QJsonObject parent;
                parent[Protocol::USER_ID] = query.value("user_id").value<qint64>();
                parent[Protocol::USER_NAME] = query.value("user_name").toString();

                parents.push_back(parent);
            }
            responseObj[Protocol::PARENTS] = parents;
            responseObj[Protocol::RESULT] = Protocol::RESULT_SUCCESS;
        }
        else
        {
            MyDebug() << "get children parents FAILED" << query.lastError().text();
            responseObj.insert(Protocol::RESULT, Protocol::RESULT_FAIL);
        }

        connectionHandler->sendMessage(responseObj);

        break;
    }
    case Protocol::Codes::CreateTest:
    {
        QSqlQuery query(db);

        QJsonArray questions = obj.value(Protocol::TEST_QUESTIONS).toArray();
        QString testName = obj.value(Protocol::TEST_NAME).toString();

        query.prepare("INSERT INTO tests_for_parents (test_name, test_questions, test_creator_id) "
                      "VALUES(:test_name, :test_questions, :test_creator_id)");
        query.bindValue(":test_name", testName);
        query.bindValue(":test_questions", QString(QJsonDocument(QJsonObject{{Protocol::TEST_QUESTIONS, questions}}).toJson(QJsonDocument::Compact)));
        query.bindValue(":test_creator_id", connectionHandler->getUserId());

        QJsonObject responseObj;
        responseObj.insert(Protocol::MESSAGE_TYPE, type);
        if(query.exec())
        {
            responseObj.insert(Protocol::RESULT, Protocol::RESULT_SUCCESS);
        }
        else
        {
            MyDebug() << "insert new test FAILED" << query.lastError().text();
            responseObj.insert(Protocol::RESULT, Protocol::RESULT_FAIL);
        }

        connectionHandler->sendMessage(responseObj);

        break;
    }
    case Protocol::Codes::GetTests:
    {
        QJsonObject tests = getTestsForParent(connectionHandler->getUserId());

        tests.insert(Protocol::MESSAGE_TYPE, type);

        if(tests.isEmpty())
        {
            tests.insert(Protocol::RESULT, Protocol::RESULT_FAIL);
        }
        else
        {
            tests.insert(Protocol::RESULT, Protocol::RESULT_SUCCESS);
        }

        connectionHandler->sendMessage(tests);

        break;
    }
    case Protocol::Codes::UploadTest:
    {
        QSqlQuery query(db);

        QJsonArray answers = obj.value(Protocol::TEST_ANSWERS).toArray();
        UserIdType testId = obj.value(Protocol::TEST_ID).toVariant().value<UserIdType>();

        query.prepare("INSERT INTO finished_tests (test_id, answers, executor_id) "
                      "VALUES(:test_id, :answers, :executor_id)");
        query.bindValue(":test_id", testId);
        query.bindValue(":answers", QString(QJsonDocument(QJsonObject{{Protocol::TEST_ANSWERS, answers}}).toJson(QJsonDocument::Compact)));
        query.bindValue(":executor_id", connectionHandler->getUserId());

        QJsonObject responseObj;
        responseObj.insert(Protocol::MESSAGE_TYPE, type);
        if(query.exec())
        {
            responseObj.insert(Protocol::RESULT, Protocol::RESULT_SUCCESS);
        }
        else
        {
            MyDebug() << "insert finished FAILED" << query.lastError().text();
            responseObj.insert(Protocol::RESULT, Protocol::RESULT_FAIL);
        }

        connectionHandler->sendMessage(responseObj);
        break;
    }
    case Protocol::AddUser:
    {
        QString userName = obj.value(USER_NAME).toString();
        QString userLogin = obj.value(USER_LOGIN).toString();
        QString userPassword = obj.value(USER_PASSWORD).toString();
        UserRole userRole = UserRole(obj.value(USER_ROLE).toInt());

        bool result = createUser(userLogin, userPassword, userName, userRole);

        QJsonObject responseObj;
        responseObj.insert(Protocol::MESSAGE_TYPE, type);
        if(result)
        {
            responseObj.insert(Protocol::RESULT, Protocol::RESULT_SUCCESS);
        }
        else
        {
            responseObj.insert(Protocol::RESULT, Protocol::RESULT_FAIL);
        }

        connectionHandler->sendMessage(responseObj);
        break;
    }
    case Protocol::AddChild:
    {
        QString name = obj.value(CHILD_NAME).toString();
        quint8 age = obj.value(CHILD_AGE).toInt();
        Gender gender = Gender(obj.value(CHILD_GENDER).toInt());
        UserIdType parentID = obj.value(CHILD_PARENT_ID).toVariant().value<qint64>();
        UserIdType teacherID = obj.value(CHILD_TEACHER_ID).toVariant().value<qint64>();

        bool result = createChild(name, age, gender, parentID, teacherID);

        QJsonObject responseObj;
        responseObj.insert(Protocol::MESSAGE_TYPE, type);
        if(result)
        {
            responseObj.insert(Protocol::RESULT, Protocol::RESULT_SUCCESS);
        }
        else
        {
            responseObj.insert(Protocol::RESULT, Protocol::RESULT_FAIL);
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

bool DatabaseAccessor::createChild(const QString &name, quint8 age, Gender gender, UserIdType parentId, UserIdType teacherId)
{
    MyDebug() << Q_FUNC_INFO;

    QSqlQuery query(db);

    query.prepare(QStringLiteral("INSERT INTO children (name, age, gender, parent_id, teacher_id) "
                                 "VALUES(:name, :age, :gender, :parent_id, :teacher_id)"));
    query.bindValue(":name", name);
    query.bindValue(":age", age);
    query.bindValue(":gender", gender);
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

void DatabaseAccessor::addTrafficDays(UserIdType childId, int year, int month, const QList<int> &addingDays)
{
    auto existingDays = getTrafficDays(childId, year, month);
    if(existingDays)
    {
        auto days = existingDays.value();
        for(int addDay: addingDays)
        {
            if(!days.contains(addDay))
            {
                days.append(addDay);
            }
        }

        QString daysStr("{");
        for(int i = 0; i < days.size(); ++i)
        {
            daysStr += QString::number(days.at(i));
            if(i != days.size() - 1)
            {
                daysStr += ",";
            }
        }
        daysStr += "}";

        QSqlQuery query(db);
        query.prepare("UPDATE children_traffic SET days=:days WHERE child_id=:child_id AND year=:year AND month=:month");
        query.bindValue(":days", daysStr);
        query.bindValue(":child_id", childId);
        query.bindValue(":year", year);
        query.bindValue(":month", month);

        if(query.exec())
        {
            MyDebug() << "add days SUCCESS";
        }
        else
        {
            MyDebug() << "add days FAILED" << query.lastError().text();
        }
    }
    return;
}

std::optional<QList<int>> DatabaseAccessor::getTrafficDays(UserIdType childId, int year, int month)
{
    QSqlQuery query(db);
    query.prepare(QStringLiteral("SELECT days FROM children_traffic WHERE child_id=:child_id AND year=:year AND month=:month"));
    query.bindValue(":child_id", childId);
    query.bindValue(":year", year);
    query.bindValue(":month", month);

    if(query.exec())
    {
        if(query.first())
        {
            QString myStr = query.value("days").toString();
            QStringRef sref (&myStr, 1, myStr.length() - 2); // Remove braces
            QVector<QStringRef> v = sref.split(",");
            QList<int> days;
            for (const QStringRef &r: v) {
                days << r.toInt();
            }
            return days;
        }
        return {QList<int>{}};
    }
    MyDebug() << "get days FAILED" << query.lastError().text();
    return {};
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
            user[Protocol::USER_LOGIN] = query.value("user_login").toString();
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

QJsonArray DatabaseAccessor::getAllUsers()
{
    QSqlQuery query(db);

    QJsonArray users;

    query.prepare(QStringLiteral("SELECT user_id, user_name, user_login, user_role FROM users"));

    if(query.exec())
    {
        QJsonObject user;

        while (query.next())
        {
            user[Protocol::USER_NAME] = query.value("user_name").toString();
            user[Protocol::USER_LOGIN] = query.value("user_login").toString();
            user[Protocol::USER_ID] = query.value("user_id").toString();
            user[Protocol::USER_ROLE] = query.value("user_role").toInt();

            users.push_back(user);
        }
    }
    else
    {
        MyDebug() << "get all users FAILED" << query.lastError().text();
    }
    return users;
}

QJsonArray DatabaseAccessor::getChildren(UserRole userRole, UserIdType id)
{
    QSqlQuery query(db);
    QJsonArray children;

    QString queryText = "SELECT id, name, age, gender, parent_id, teacher_id FROM children";
    if(userRole != UserRole::Admin)
    {
        if(userRole == UserRole::Parent)
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
            child[Protocol::CHILD_AGE] = query.value("age").toInt();
            child[Protocol::CHILD_ID] = query.value("id").toString();
            child[Protocol::CHILD_PARENT_ID] = query.value("parent_id").value<qint64>();
            child[Protocol::CHILD_TEACHER_ID] = query.value("teacher_id").value<qint64>();
            child[Protocol::CHILD_GENDER] = query.value("gender").toInt();

            children.push_back(child);
        }
    }
    else
    {
        MyDebug() << "get all children FAILED" << query.lastError().text();
    }
    return children;
}

QJsonArray DatabaseAccessor::getAllChildren()
{
    return getChildren();
}


QJsonArray DatabaseAccessor::getChildrenForTeacher(UserIdType id)
{
    return getChildren(UserRole::Teacher, id);
}

QJsonArray DatabaseAccessor::getChildrenForParent(UserIdType id)
{
    return getChildren(UserRole::Parent, id);
}

QJsonObject DatabaseAccessor::getTestsForParent(UserIdType parentId)
{
    QSqlQuery query(db);
    query.prepare("SELECT test_id, test_name, test_questions FROM tests_for_parents WHERE test_creator_id IN "
                  "(SELECT teacher_id FROM children WHERE parent_id=:parent_id) AND test_id NOT IN "
                  "(SELECT test_id FROM finished_tests WHERE executor_id=:parent_id)");
    query.bindValue(":parent_id", parentId);

    QJsonObject testsObj;

    if(query.exec())
    {
        QJsonArray array;
        while (query.next())
        {
            QJsonObject testObj = QJsonDocument::fromJson(query.value(Protocol::TEST_QUESTIONS).toByteArray()).object();
            testObj.insert(Protocol::TEST_NAME, query.value("test_name").toString());
            testObj.insert(Protocol::TEST_ID, query.value("test_id").value<qint64>());
            array.push_back(testObj);
        }
        testsObj.insert(Protocol::TESTS, array);
    }
    else
    {
        MyDebug() << "get tests FAILED" << query.lastError().text();
    }
    return testsObj;
}
