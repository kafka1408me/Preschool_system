#pragma once

#include <QAbstractListModel>
#include "Codes.h"

class UsersModel;

#define ADD_MEMBER(type, name, getter, setter) \
    private: \
        type name; \
    public: \
    type getter () const {return name;} \
    void setter(const type& x){name = x;}


class Child
{
public:
    Child() = default;

    ADD_MEMBER(UserIdType, m_id, getId, setId)
    ADD_MEMBER(UserIdType, m_parentId, getParentId, setParentId)
    ADD_MEMBER(UserIdType, m_teacherId, getTeacherId, setTeacherId)
    ADD_MEMBER(quint8, m_age, getAge, setAge)
    ADD_MEMBER(Gender, m_gender, getGender, setGender)
    ADD_MEMBER(QString, m_name, getName, setName)
    ADD_MEMBER(QList<int>, m_visitDays, getVisitDays, setVisitDays)
};

class ChildrenModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles{
        ChildID = Qt::UserRole + 1,
        ChildParentId,
        ChildTeacherId,
        ChildAge,
        ChildGender,
        ChildName,
        ChildParentName,
        ChildTeacherName,
        ChildVisitDays
    };

    explicit ChildrenModel(UsersModel* usersModel, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    void setChildren(const QJsonArray& array);

    void setChildVisitDays(UserIdType id, const QList<int>& visitDays);

    void clear();

    QVariantMap getItemMap(int _index);

private:
    UsersModel* m_usersModel;
    QVector<Child> m_children;
};


