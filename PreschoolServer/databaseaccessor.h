#pragma once

#include <QObject>
#include <QSqlDatabase>

class DatabaseAccessor: public QObject
{
    Q_OBJECT
public:
    DatabaseAccessor(QObject* parent = nullptr);

    ~DatabaseAccessor();

public slots:
    void start();

private:
    QSqlDatabase db;
};


