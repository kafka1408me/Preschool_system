import QtQuick 2.12
import "qrc:/Functions.js" as Functions

Item {
    id: idRoot

    signal showMessage(string text)

    signal testCreated()

    Text {
        id: idTitleText
        anchors.horizontalCenter: parent.horizontalCenter
        y: 30
        color: "black"
        font {
            pointSize: 20
            weight: Font.Bold
        }
        text: "Создать тест"
    }

    Rectangle {
        id: idLineRectangle
        anchors.horizontalCenter: parent.horizontalCenter
        y: idTitleText.y + idTitleText.contentHeight + 30
        width: parent.width * 0.9
        height: 2
        color: "#b6b6b6"
    }

    InputFieldWithDescription {
        id: idNameTestInputField
        anchors.left: idLineRectangle.left
        y: idLineRectangle.y + idLineRectangle.height + 20

        inputFieldWith: 310
        descriptionText: "Название теста"
    }

    MyButton {
        id: idFinishCreateTestButton
        btnText: "Завершить создание теста"
        anchors.verticalCenter: idNameTestInputField.verticalCenter
        x: idNameTestInputField.x + idNameTestInputField.width + 20
        width: 250
        height: 50
        visible: idNameTestInputField.text.length != 0 && idQuestionsView.count != 0
        btnColor: Functions.mainColor
        btnRadius: 10

        onClicked: {
            var questions = []
            for(var i = 0; i < idQuestionsModel.count; ++i)
            {
                questions.push(idQuestionsModel.get(i).text)
            }
            connection.createTest(idNameTestInputField.text, questions)
            idRoot.testCreated()
        }
    }

    ListModel {
        id: idQuestionsModel
    }

    Text {
        id: idQuestionsText
        text: "Вопросы:"
        visible: idQuestionsView.count > 0
        anchors.left: idLineRectangle.left
        y: idNameTestInputField.y + idNameTestInputField.height + 15
        font {
            pointSize: Functions.mainFontPointSize
            weight: Font.Bold
        }
    }

    ListView {
        id: idQuestionsView
        anchors {
            left: idLineRectangle.left
            right: idLineRectangle.right
        }
        y: idQuestionsText.y + idQuestionsText.contentHeight + 13
        height: idAddButton.y - y - 15
        spacing: 15

        model: idQuestionsModel

        delegate: Item {
            height: 30
            width: idQuestionsView.width

            Text {
                x: 5
                anchors.verticalCenter: parent.verticalCenter
                text: model.text
                font {
                    pointSize: 14
                }
                color: "black"
            }

            Rectangle {
                anchors {
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                }
                height: 1
                color: "#cccccc"
            }
        }
    }

    InputFieldWithDescription {
        id: idQuestionInputField
        x: idAddButton.x - width - 10
        y: idAddButton.y
        fieldHeight: idAddButton.height
        inputFieldWith: 250
        descriptionText: "Вопрос"
        fieldBackgroundBorderColor: "#afafaf"
        fieldBackgroundBorderFocusColor: Qt.darker(fieldBackgroundBorderColor, 1.1)
        descriptionTextColor: fieldBackgroundBorderColor
    }

    MyButtonWithPlus {
        id: idAddButton
        y: idExitButton.y
        x: idExitButton.x - idLeftAreaRectangle.width - width - 30
        btnText: "Добавить вопрос"
        btnRadius: 5
        height: 50
        width: 220

        onClicked: {
            if(idQuestionInputField.text.length == 0)
            {
                showMessage("Заполните вопрос")
            }
            else
            {
                var question = {"text": idQuestionInputField.text}
                idQuestionsModel.append(question)
                idQuestionInputField.text = ""
            }
        }
    }

}
