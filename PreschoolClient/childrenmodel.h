#pragma once

#include <QAbstractListModel>
#include "Codes.h"

class UsersModel;

class Child
{
    Child(UserIdType id, UserIdType parentId, UserIdType teacher_id, quint8 age, Gender gender, const QString& name);

private:
    UserIdType m_id;
    UserIdType m_parentId;
    UserIdType m_teacherId;
    quint8 m_age;
    Gender m_gender;
    QString m_name;
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
        ChildParentName,
        ChildTeacherName
    };

    explicit ChildrenModel(UsersModel* usersModel, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    void setChildren(const QJsonArray& array);

private:
    UsersModel* m_usersModel;
    QVector<Child> m_children;
};


