#pragma once

#include <QSortFilterProxyModel>
#include "Codes.h"

class UsersProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit UsersProxyModel(QAbstractItemModel* _sourceModel, QObject *parent = nullptr);

    void setUsersRole(User::UserRole role);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    User::UserRole m_usersRole{UserRole::Parent};
};

