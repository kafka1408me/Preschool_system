#include "usersproxymodel.h"
#include "usersmodel.h"

UsersProxyModel::UsersProxyModel(QAbstractItemModel *_sourceModel, QObject *parent) : QSortFilterProxyModel(parent)
{
    setSourceModel(_sourceModel);
    setFilterRole(UsersModel::UserRole);
}

void UsersProxyModel::setUsersRole(User::UserRole role)
{
    m_usersRole = role;
    invalidateFilter();
}

bool UsersProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    auto index = sourceModel()->index(sourceRow, 0, sourceParent);
    if (index.isValid())
    {
            QVariant data = sourceModel()->data(index, filterRole());
            return data.toInt() == m_usersRole;
    }

    return false;
}
