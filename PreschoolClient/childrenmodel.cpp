#include <QJsonArray>
#include <QJsonObject>
#include "childrenmodel.h"
#include "usersmodel.h"


ChildrenModel::ChildrenModel(UsersModel *usersModel, QObject *parent) :
    QAbstractListModel(parent),
    m_usersModel(usersModel)
{
    connect(m_usersModel, &UsersModel::modelReset, this, [this] {
        beginResetModel();
        endResetModel();
    });
}

int ChildrenModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_children.size();
}

QVariant ChildrenModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() >= rowCount())
    {
        return {};
    }

    const Child& child = m_children.at(index.row());

    switch (role)
    {
    case ChildID:
    {
        return child.getId();
    }
    case ChildParentId:
    {
        return child.getParentId();
    }
    case ChildTeacherId:
    {
        return child.getTeacherId();
    }
    case ChildAge:
    {
        return child.getAge();
    }
    case ChildGender:
    {
        return child.getGender();
    }
    case ChildName:
    {
        return child.getName();
    }
    case ChildParentName:
    {
        auto user = m_usersModel->getUserById(child.getParentId());
        if(user)
        {
            return user->getUserName();
        }
        return "";
    }
    case ChildTeacherName:
    {
        auto user = m_usersModel->getUserById(child.getTeacherId());
        if(user)
        {
            return user->getUserName();
        }
        return "";
    }
    case ChildVisitDays:
    {
        QList<int> visitDays{1,2, 13, 14, 15};// = child.getVisitDays(); TEST
        QVariantList days;
        days.reserve(visitDays.size());

        for(int day: visitDays)
        {
            days.append(day);
        }
        return days;
    }
    default:
    {
    }
    }
    return {};
}

QHash<int, QByteArray> ChildrenModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ChildID] = "id";
    roles[ChildParentId] = "parentId";
    roles[ChildTeacherId] = "teacherId";
    roles[ChildAge] = "age";
    roles[ChildGender] = "gender";
    roles[ChildName] = "name";
    roles[ChildParentName] = "parentName";
    roles[ChildTeacherName] = "teacherName";
    roles[ChildVisitDays] = "visitDays";

    return roles;
}

void ChildrenModel::setChildren(const QJsonArray &array)
{
    beginResetModel();

    m_children.clear();
    m_children.reserve(array.size());

    QJsonObject obj;
    Child child;
    for(const QJsonValue& jsonVal: array)
    {
        obj = jsonVal.toObject();

        child.setId(obj.value(Protocol::CHILD_ID).toVariant().value<UserIdType>());
        child.setName(obj.value(Protocol::CHILD_NAME).toString());
        child.setAge(obj.value(Protocol::CHILD_AGE).toInt());
        child.setGender(Gender(obj.value(Protocol::CHILD_GENDER).toInt()));
        child.setParentId(obj.value(Protocol::CHILD_PARENT_ID).toVariant().value<UserIdType>());
        child.setTeacherId(obj.value(Protocol::CHILD_TEACHER_ID).toVariant().value<UserIdType>());

        m_children.push_back(child);
    }

    qDebug() << "count children" << m_children.size();

    endResetModel();
}

void ChildrenModel::setChildVisitDays(UserIdType id, const QList<int> &visitDays)
{
    int row = -1;
    auto it = std::find_if(m_children.begin(), m_children.end(), [id, &row](const Child& child){
        ++row;
        return child.getId() == id;
    });

    if(it != m_children.end())
    {
        it->setVisitDays(visitDays);
        emit dataChanged(index(row, 0), index(row, 0));
    }
}

void ChildrenModel::clear()
{
    beginResetModel();
    m_children.clear();
    endResetModel();
}

QVariantMap ChildrenModel::getItemMap(int _index)
{
    QVariantMap varMap;
    if(_index < 0 || _index >= m_children.size())
    {
        return varMap;
    }

    const auto roles = roleNames();

    for(auto it = roles.cbegin(), end = roles.cend(); it != end; ++it)
    {
        varMap[it.value()] = data(index(_index, 0), it.key());
    }

    return varMap;
}

