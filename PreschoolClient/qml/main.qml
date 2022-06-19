import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

import preschool 1.0

Window {
    id: idMainWindow
    width: Screen.width * 0.62
    height: width * 0.7
    visible: true

    Row{
        id: idConnectionStatusRow
        x: parent.width * 0.01
        y: 30
        z: 100
        spacing: 15
        Rectangle {
            id: idConnectionRectangle
            width: 30
            height: width
            radius: height / 2
            color: connection.isConnected ? "#2ff12f" : "#ef4e42"
        }
        Text {
            anchors.verticalCenter: idConnectionRectangle.verticalCenter
            color: "white"
            font.pointSize: 14
            font.weight: Font.Bold
            text: connection.isConnected ? "Online" : "Offline"
        }
    }

    StackView {
        id: idStackView
        anchors.fill: parent
        initialItem: "LoginPage.qml"

        Connections {
            target: idStackView.currentItem
            ignoreUnknownSignals: true

            function onShowMessage(text) {
                idMessageRectangle.showMessage(text)
            }

            function onLogInSuccess() {
                idStackView.replace("MainServicePage.qml")
                if(userInfo.userRole === UserRole.Admin)
                {
                    connection.getAllUsers()
                }
                connection.getChildren()

                if(userInfo.userRole === UserRole.Parent)
                {
                    connection.getChildTeacher()
                }
            }

            function onLogOut() {
                idStackView.replace("LoginPage.qml")
                userInfo.clearData()
                connection.logOut()
            }
        }
    }

    Rectangle {
        id: idMessageRectangle
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter:  idConnectionStatusRow.verticalCenter
        z: 100
        width: idMessageText.contentWidth + 30
        height: 40
        visible: false
        color: "#ececec"
        radius: 7

        function showMessage(text) {
            idMessageText.text = text
            visible = true
            idMessageShowTimer.restart()
        }

        Text {
            id: idMessageText
            anchors.horizontalCenter: idMessageRectangle.horizontalCenter
            anchors.verticalCenter: idMessageRectangle.verticalCenter
            color: "#444343"
            font.pointSize: 14
        }

        Timer {
            id: idMessageShowTimer
            interval: 1500
            repeat: false
            running: false

            onTriggered: {
                idMessageRectangle.visible = false
            }
        }
    }

}
