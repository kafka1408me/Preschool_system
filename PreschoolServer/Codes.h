#pragma once

#include <QString>

using UserIdType = quint64;
#include <QObject>

#define ADD_MEMBER(type, name, getter, setter) \
    private: \
        type name; \
    public: \
    type getter () const {return name;} \
    void setter(const type& x){name = x;}


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
enum Gender
{
    Male   = 1,
    Female = 2
};
Q_ENUM_NS(Gender)
}

using namespace User;


namespace Protocol
{

enum Codes
{
    Authorization   = 1,
    LogOut          = 2,
    GetChildren     = 3,
    GetAllUsers     = 4,
    GetChildTeacher = 5,
    GetChildrenParents = 6,
    CreateTest         = 7,
    GetTests           = 8,
    UploadTest         = 9,
    AddUser            = 10,
    AddChild           = 11
};

const QString MESSAGE_TYPE = "type";
const QString USER_LOGIN = "login";
const QString PASSWORD = "password";
const QString MESSAGE_DATA = "main";
const QString RESULT = "result";

const QString USER_NAME = "user_name";
const QString USER_ROLE = "user_role";
const QString USER_ID = "user_id";
const QString USER_PASSWORD = "user_password";

const QString USERS = "users";
const QString CHILDREN = "children";

const QString CHILD_ID = "id";
const QString CHILD_NAME = "name";
const QString CHILD_AGE = "age";
const QString CHILD_PARENT_ID = "parent_id";
const QString CHILD_TEACHER_ID = "teacher_id";
const QString CHILD_GENDER = "gender";

const QString CHILDREN_PARENTS_IDS = "children_parent";
const QString PARENTS = "parents";

const QString TEST_NAME = "test_name";
const QString TEST_QUESTIONS = "test_questions";
const QString TEST_ANSWERS = "test_answers";
const QString QUESTION_TEXT = "question_text";
const QString TEST_ID = "test_id";
const QString TESTS = "tests";

constexpr int RESULT_SUCCESS = 0;
constexpr int RESULT_FAIL = 1;
}
