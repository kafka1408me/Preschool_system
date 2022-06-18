#include <QThread>
#include "connectionwrapper.h"
#include "connection.h"


ConnectionWrapper::ConnectionWrapper(const QUrl &url, QObject *parent):
    QObject(parent)
{
    m_connection.reset(new Connection(url));
    m_connectionThread = new QThread(this);

    connect(this, &ConnectionWrapper::startConnecting,
            m_connection.get(), &Connection::start, Qt::QueuedConnection);
    connect(m_connection.get(), &Connection::isConnectedChanged,
            this, &ConnectionWrapper::onConnectedChanged, Qt::QueuedConnection);
    connect(this, &ConnectionWrapper::tryLogIn,
            m_connection.get(), &Connection::logIn, Qt::QueuedConnection);

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

void ConnectionWrapper::onConnectedChanged(bool connected)
{
    m_isConnected = connected;
    emit isConnectedChanged(m_isConnected);
}
