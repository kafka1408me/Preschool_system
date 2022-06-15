#pragma once

#include <QCoreApplication>

class QSettings;

class Application: public QCoreApplication
{
public:
    Application(int argc, char** argv);

    static Application* getInstance();

    int getServerPort() const;

    QString getDatabaseHost() const;

    QString getDatabaseName() const;

    QString getDatabaseUser() const;

    QString getDatabasePassword() const;

private:
    QSettings* m_settings;
};

#define MyApp Application::getInstance()

