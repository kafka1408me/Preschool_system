#include <QThread>
#include "connectionwrapper.h"
#include "connection.h"

#define MyDebug() qDebug() << "ConnectionWrapper:"

ConnectionWrapper::ConnectionWrapper(const QUrl &url, QObject *parent):
    QObject(parent)
{
    m_connection.reset(new Connection(url));
    m_connectionThread = new QThread(this);

    connect(this, &ConnectionWrapper::startConnecting,
            m_connection.get(), &Connection::start, Qt::QueuedConnection);
    connect(this, &ConnectionWrapper::tryLogIn,
            m_connection.get(), &Connection::logIn, Qt::QueuedConnection);
    connect(this, &ConnectionWrapper::tryLogOut,
            m_connection.get(), &Connection::logOut, Qt::QueuedConnection);
    connect(this, &ConnectionWrapper::tryGetAllUsers,
            m_connection.get(), &Connection::getAllUsers, Qt::QueuedConnection);
    connect(this, &ConnectionWrapper::tryGetChildren,
            m_connection.get(), &Connection::getChildren, Qt::QueuedConnection);
    connect(this, &ConnectionWrapper::tryGetChildTeacher,
            m_connection.get(), &Connection::getChildTeacher, Qt::QueuedConnection);
    connect(this, &ConnectionWrapper::tryGetChildrenParents,
            m_connection.get(), &Connection::getChildrenParents, Qt::QueuedConnection);
    connect(this, &ConnectionWrapper::tryCreateTest,
            m_connection.get(), &Connection::createTest, Qt::QueuedConnection);

    connect(m_connection.get(), &Connection::isConnectedChanged,
            this, &ConnectionWrapper::onConnectedChanged, Qt::QueuedConnection);
    connect(m_connection.get(), &Connection::logInSuccess,
            this, &ConnectionWrapper::logInSuccess);
    connect(m_connection.get(), &Connection::logInFailed,
            this, &ConnectionWrapper::logInFailed);
    connect(m_connection.get(), &Connection::showMessage,
            this, &ConnectionWrapper::showMessage);

    m_connection->moveToThread(m_connectionThread);
    m_connectionThread->start();
}

ConnectionWrapper::~ConnectionWrapper()
{
    m_connectionThread->quit();
    m_connectionThread->wait();
}

void ConnectionWrapper::start()
{
    emit startConnecting();
}

bool ConnectionWrapper::isConnected() const
{
    return m_isConnected;
}

void ConnectionWrapper::logIn(const QString &login, const QString &password)
{
    emit tryLogIn(login, password);
}

void ConnectionWrapper::logOut()
{
    emit tryLogOut();
}

void ConnectionWrapper::getAllUsers()
{
    emit tryGetAllUsers();
}

void ConnectionWrapper::getAllChildren()
{
    emit tryGetAllChildren();
}

void ConnectionWrapper::getChildren()
{
    emit tryGetChildren();
}

void ConnectionWrapper::getChildTeacher()
{
    emit tryGetChildTeacher();
}

void ConnectionWrapper::getChildrenParents()
{
    emit tryGetChildrenParents();
}

void ConnectionWrapper::createTest(QString testName, QVariantList questions)
{
    MyDebug() << Q_FUNC_INFO;

    QStringList questionsStrList;
    questionsStrList.reserve(questions.size());
    std::transform(questions.cbegin(), questions.cend(), std::back_inserter(questionsStrList), [](const QVariant& var) {
        return var.toString();
    });

    emit tryCreateTest(testName, questionsStrList);
}

void ConnectionWrapper::onConnectedChanged(bool connected)
{
    m_isConnected = connected;
    emit isConnectedChanged(m_isConnected);
}
