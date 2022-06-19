#include "childrenmodel.h"
#include "usersmodel.h"

ChildrenModel::ChildrenModel(UsersModel *usersModel, QObject *parent) :
    QAbstractListModel(parent),
    m_usersModel(usersModel)
{
}

int ChildrenModel::rowCount(const QModelIndex &parent) const
{
    return m_children.size();
}

QVariant ChildrenModel::data(const QModelIndex &index, int role) const
{

}

QHash<int, QByteArray> ChildrenModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ChildID] = "id";
    roles[ChildParentId] = "parentId";
    roles[ChildTeacherId] = "teacherId";
    roles[ChildAge] = "age";
    roles[ChildGender] = "gender";

    return roles;
}

void ChildrenModel::setChildren(const QJsonArray &array)
{

}
