#pragma once

#include <QString>

using UserIdType = quint64;
#include <QObject>


const UserIdType DefaultUserId{0};

constexpr const int DefaultPort = 19653;

namespace User
{
Q_NAMESPACE
enum UserRole
{
    Parent  = 1,
    Teacher = 2,
    Admin   = 3
};
Q_ENUM_NS(UserRole)
}


namespace Protocol
{

enum Codes
{
    Authorization = 1
};

const QString MESSAGE_TYPE = "type";
const QString LOGIN = "login";
const QString PASSWORD = "password";
const QString MESSAGE_DATA = "main";
const QString RESULT = "result";

constexpr int RESULT_SUCCESS = 0;
constexpr int RESULT_FAIL = 1;
}
