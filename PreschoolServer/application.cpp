#include <QSettings>
#include "application.h"

static Application* application{nullptr};
constexpr int defaultPort = 18751;

Application::Application(int argc, char **argv):
    QCoreApplication(argc, argv)
{
    m_settings = new QSettings("config.ini", QSettings::IniFormat, this);

    application = this;
}

Application* Application::getInstance()
{
    return application;
}

int Application::getServerPort() const
{
    return m_settings->value(QStringLiteral("port"), defaultPort).toInt();
}

QString Application::getDatabaseHost() const
{
    return m_settings->value(QStringLiteral("database_ip")).toString();
}

QString Application::getDatabaseName() const
{
    return m_settings->value(QStringLiteral("database_name")).toString();
}

QString Application::getDatabaseUser() const
{
    return m_settings->value(QStringLiteral("database_user")).toString();
}

QString Application::getDatabasePassword() const
{
    return m_settings->value(QStringLiteral("database_password")).toString();
}
