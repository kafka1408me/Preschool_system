#include <QDebug>
#include <QSqlError>
#include "databaseaccessor.h"
#include "application.h"

#define MyDebug() qDebug() << "DatabaseAccessor:"

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
        MyDebug() << "FAILURE connection to database: " << db.lastError();
        exit(-100);
    }
}
