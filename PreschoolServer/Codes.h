#pragma once

#include <QString>

using UserIdType = quint64;

const UserIdType DefaultUserId{0};

namespace Protocol
{

enum Codes
{
    Authorization = 1
};

const QString MESSAGE_TYPE = "type";

}
