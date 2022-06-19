#include "proxychildrenmodel.h"
#include "childrenmodel.h"

ProxyChildrenModel::ProxyChildrenModel(QAbstractItemModel* _sourceModel, QObject *parent):
    QSortFilterProxyModel(parent)
{
    setSourceModel(_sourceModel);
    setFilterRole(ChildrenModel::Roles::ChildID);
}

void ProxyChildrenModel::setId(UserIdType id)
{
    m_childId = id;
    invalidateFilter();
}

bool ProxyChildrenModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if(m_childId == DefaultUserId)
    {
        return true;
    }

    auto index = sourceModel()->index(sourceRow, 0, sourceParent);
    if (index.isValid())
    {
            QVariant data = sourceModel()->data(index, filterRole());
            return data.value<UserIdType>() == m_childId;
    }

    return false;
}
