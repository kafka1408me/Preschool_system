import QtQuick 2.12
import "qrc:/Functions.js" as Functions

Item {
    id: idRoot
    anchors.fill: parent

    signal showMessage(string text)

    Rectangle {
        id: idTopRectangle
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: parent.height * 0.6
        color: Functions.mainColor
        Image {
            id: idChildren
            anchors.horizontalCenter: parent.horizontalCenter
            y: parent.height - height - parent.height*0.05
            width: parent.width * 0.45
            fillMode: Image.PreserveAspectFit
            source: "qrc:/images/children.jpg"
            asynchronous: true
        }
    }
    Item {
        id: idBottomArea
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: idTopRectangle.bottom
        anchors.bottom: parent.bottom
        Column {
            id: idInputFieldsArea
            anchors.horizontalCenter: parent.horizontalCenter
            y: 30
            spacing: 20
            InputFieldWithDescription {
                id: idLoginInputField
                inputFieldWith: idBottomArea.width * 0.3
                descriptionText: "Логин"
            }
            InputFieldWithDescription {
                id: idPasswordInputField
                inputFieldWith: idLoginInputField.inputFieldWith
                descriptionText: "Пароль"
                echoMode: TextInput.Password
            }
        }

        MyButton {
            width: 120
            height: 40
            x: idInputFieldsArea.x + idPasswordInputField.x + idPasswordInputField.width - width
            y: idInputFieldsArea.y + idInputFieldsArea.height + 50
            btnText: "Войти"
            btnColor: Functions.mainColor

            onClicked: {
                console.log("enter button clicked")
                if(connection.isConnected)
                {
                    connection.logIn(idLoginInputField.text, idPasswordInputField.text)
                }
                else
                {
                    idRoot.showMessage("Нет соединения с сервером")
                }
            }
        }
    }
}
