#ifndef TESTSMODEL_H
#define TESTSMODEL_H

#include <QAbstractListModel>
#include "Codes.h"

class Test
{
    ADD_MEMBER(UserIdType, m_id, getId, setId)
    ADD_MEMBER(QString, m_name, getName, setName)
    ADD_MEMBER(QStringList, m_questions, getQuestions, setQuestions)
};

class TestsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles{
        TestID = Qt::UserRole + 1,
        TestName,
        TestQuestions
    };

    explicit TestsModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    void setTests(const QJsonArray& array);

    void clear();

    QVariantMap getItemMap(int _index);

private:
    QVector<Test> m_tests;
};

#endif // TESTSMODEL_H
