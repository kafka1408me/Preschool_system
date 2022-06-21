import QtQuick 2.0

Item {
    id: idRoot

    signal showMessage(string text)

    signal passTest(int index)

    TitleText {
        id: idTitleText
        text: "Тесты"
    }

    Rectangle {
        id: idLineRectangle
        anchors.horizontalCenter: parent.horizontalCenter
        y: idTitleText.y + idTitleText.contentHeight + 30
        width: parent.width * 0.9
        height: 2
        color: "#b6b6b6"
    }

    Text {
        visible: idTestsView.count === 0
        anchors.horizontalCenter: parent.horizontalCenter
        y: idLineRectangle.y + 50
        text: "Для Вас тестов нет"
        font {
            pointSize: 24
            weight: Font.DemiBold
        }
        color: "#9d9d9d"
    }

    ListView {
        id: idTestsView
        anchors {
            left: idLineRectangle.left
            right: idLineRectangle.right
        }
        y: idLineRectangle.y + idLineRectangle.height + 13
        height: idExitButton.y - y - 15
        spacing: 15

        model: userInfo.testsModel

        delegate: Item {
            height: idPassTestButton.height + 16
            width: idTestsView.width

            Text {
                x: 5
                anchors.verticalCenter: parent.verticalCenter
                text: model.testName
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

            MyButton {
                id: idPassTestButton
                anchors.verticalCenter: parent.verticalCenter
                x: parent.width - width - 10
                width: 160
                height: 50
                btnColor: "#45c1f1"
                btnPressedColor: Qt.darker(btnColor, 1.06)
                btnText: "Пройти тест"
                btnBorderWidth: 3
                btnBorderColor: "#44a4ec"
                btnRadius: 7

                onClicked: {
                    idRoot.passTest(model.index)
                }
            }
        }
    }
}
