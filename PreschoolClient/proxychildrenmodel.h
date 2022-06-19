#pragma once

#include <QSortFilterProxyModel>
#include "Codes.h"

class ProxyChildrenModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ProxyChildrenModel(QAbstractItemModel* _sourceModel, QObject *parent = nullptr);

    void setId(UserIdType id);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    UserIdType m_childId{DefaultUserId};
};

