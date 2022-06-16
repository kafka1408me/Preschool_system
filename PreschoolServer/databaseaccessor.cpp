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
}

void DatabaseAccessor::onRequest(const QJsonObject &obj, ConnectionHandler::ConnectionPtr connectionHandler)
{
    using namespace Protocol;

    MyDebug() << Q_FUNC_INFO << obj;
    QSqlQuery query(db);

    const int type = obj.value(MESSAGE_TYPE).toInt();

    QJsonObject mainObj = obj.value(MESSAGE_DATA).toObject();

    MyDebug() << "mainObj" << mainObj;
    switch (type)
    {
    case Codes::Authorization:
    {
        QString login = mainObj.value(LOGIN).toString();
        QString pass = mainObj.value(PASSWORD).toString();

        query.prepare(QStringLiteral("SELECT id, name, role, hash, salt FROM users "
                                     "WHERE user_login=:user_login"));
        query.bindValue(QStringLiteral(":login"), login);

        QJsonObject responseObj;

        query.exec();
        if(query.first())
        {

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
