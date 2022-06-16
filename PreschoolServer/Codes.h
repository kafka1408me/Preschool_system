#pragma once

#include <QString>

using UserIdType = quint64;

const UserIdType DefaultUserId{0};

enum UserRole
{
    Parent  = 1,
    Teacher = 2,
    Admin   = 3
};

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

constexpr int RESULT_OK = 0;
constexpr int RESULT_FAIL = 1;
}
