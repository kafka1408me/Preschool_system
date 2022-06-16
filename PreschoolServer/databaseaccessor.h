#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QJsonObject>
#include "connectionhandler.h"

class DatabaseAccessor: public QObject
{
    Q_OBJECT
public:
    DatabaseAccessor(QObject* parent = nullptr);

    ~DatabaseAccessor();

public slots:
    void start();

    void onRequest(const QJsonObject& obj);

private:
    QSqlDatabase db;
};


