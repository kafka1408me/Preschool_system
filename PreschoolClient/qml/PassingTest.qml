import QtQuick 2.0

Item {
    id: idRoot

    signal showMessage(string text)

    signal testFinished()

    property var test

    TitleText {
        id: idTitleText
        text: qsTr("Выполнение теста: %1").arg(test.testName)
    }

    Rectangle {
        id: idLineRectangle
        anchors.horizontalCenter: parent.horizontalCenter
        y: idTitleText.y + idTitleText.contentHeight + 30
        width: parent.width * 0.9
        height: 2
        color: "#b6b6b6"
    }

    ListView {
        id: idView
        anchors {
            left: idLineRectangle.left
            right: idLineRectangle.right
        }
        y: idLineRectangle.y + idLineRectangle.height + 13
        height: idExitButton.y - y - 15
        clip: true
        spacing: 15

        model: test.testQuestions

        function allAnswersFilled() {
            for(var i = 0; i < idView.count; ++i)
            {
                if(idView.itemAtIndex(i).isEmptyAnswer())
                {
                    return false
                }
            }
            return true
        }

        function getAnswers() {
            var answers = []
            for(var i = 0; i < idView.count; ++i)
            {
                var answerText = idView.itemAtIndex(i).getAnswer()
                var questionText = idView.itemAtIndex(i).getQuestion()
                var answer = {"question": questionText, "answer": answerText }
                answers.push(answer)
            }
            return answers
        }

        delegate: Item {
            height: idAnswerField.height + 16
            width: idView.width

            function isEmptyAnswer() {
                return idAnswerField.text.length === 0
            }

            function getQuestion() {
                return modelData
            }

            function getAnswer() {
                return idAnswerField.text
            }

            Text {
                x: 5
                width: idAnswerField.x - x - 5
                anchors.verticalCenter: parent.verticalCenter
                text: modelData
                font {
                    pointSize: 14
                }
                color: "black"
                elide: Text.ElideRight
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

            InputFieldWithDescription {
                id: idAnswerField
                x: parent.width - width - 5
                anchors.verticalCenter: parent.verticalCenter
                fieldHeight: 50
                inputFieldWith: 200
                descriptionText: "Ответ"
                fieldBackgroundBorderColor: "#afafaf"
                fieldBackgroundBorderFocusColor: Qt.darker(fieldBackgroundBorderColor, 1.1)
                descriptionTextColor: fieldBackgroundBorderColor
            }
        }
    }

    MyButton {
        id: idFinishedTestButton
        btnText: "Завершить тест"
        x: idExitButton.x - idLeftAreaRectangle.width - width - 30
        y: idExitButton.y
        width: 210
        height: idExitButton.height
        btnColor: "#45c1f1"
        btnBorderColor: "#44a4ec"
        btnBorderWidth: 2

        onClicked: {
            console.log("test finished button clicked")

            if(idView.allAnswersFilled())
            {
                var answers = idView.getAnswers()
                connection.uploadTest(test.testId, answers)
                idRoot.testFinished()
            }
            else
            {
                idRoot.showMessage("Заполните все ответы на вопросы")
            }
        }
    }
}
