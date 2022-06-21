#include <QJsonObject>
#include <QJsonArray>
#include "testsmodel.h"

TestsModel::TestsModel(QObject *parent) : QAbstractListModel(parent)
{

}

int TestsModel::rowCount([[maybe_unused]]const QModelIndex &parent) const
{
    return m_tests.size();
}

QVariant TestsModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() >= rowCount())
    {
        return {};
    }

    const Test& test = m_tests.at(index.row());

    switch (role)
    {
    case TestID:
    {
        return test.getId();
    }
    case TestName:
    {
        return test.getName();
    }
    case TestQuestions:
    {
        QStringList questions = test.getQuestions();
        QVariantList questionsArray;
        for(const QString& questionStr: questions)
        {
            questionsArray.push_back(questionStr);
        }
        return questionsArray;
    }
    default:
    {
        break;
    }
    }

    return {};
}

QHash<int, QByteArray> TestsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TestID] = "testId";
    roles[TestName] = "testName";
    roles[TestQuestions] = "testQuestions";

    return roles;
}

void TestsModel::setTests(const QJsonArray &array)
{
    beginResetModel();

    m_tests.clear();
    m_tests.reserve(array.size());

    QJsonObject obj;
    Test test;
    for(const QJsonValue& jsonVal: array)
    {
        obj = jsonVal.toObject();

        test.setId(obj.value(Protocol::TEST_ID).toVariant().value<UserIdType>());
        test.setName(obj.value(Protocol::TEST_NAME).toString());

        QJsonArray questions = obj.value(Protocol::TEST_QUESTIONS).toArray();
        QStringList questionsList;
        for(const QJsonValue& questionVal: questions)
        {
            QJsonObject questionObj = questionVal.toObject();
            questionsList << questionObj.value(Protocol::QUESTION_TEXT).toString();
        }
        test.setQuestions(questionsList);

        m_tests.push_back(test);
    }

    qDebug() << "count children" << m_tests.size();

    endResetModel();
}

void TestsModel::clear()
{
    beginResetModel();
    m_tests.clear();
    endResetModel();
}

QVariantMap TestsModel::getItemMap(int _index)
{
    QVariantMap varMap;
    if(_index < 0 || _index >= m_tests.size())
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
