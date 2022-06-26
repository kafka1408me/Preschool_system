import QtQuick 2.12
import QtQuick.Controls 2.12
import "qrc:/Functions.js" as Functions

import preschool 1.0

Item {
    id: idRoot

    signal showMessage(string text)
    signal logOut()

    property var tabsParent: [{name:"Мой ребенок",
            action: function(){
                var child = userInfo.getChild(0)
                if(Object.keys(child).length > 0)
                {
                    console.log("child", child.id)
                    userInfo.setChildIdForShowing(child.id)
                }
                idRightAreaStackView.replace("MyChildPage.qml")
            }},
        {name:"Тесты", action: function(){
            idRightAreaStackView.replace("TestsPage.qml")
        }},
        {name:"Оплата", action: function(){
            idRightAreaStackView.replace("PaymentPage.qml")
        }}
    ]
    property var tabsTeacher: [{name:"Дети",
        action: function(){
            idRightAreaStackView.replace("PeoplePage.qml", {"isShowUsers":false, "isTeacher":true})
        }},
        {name:"Создать тесты", action: function(){
            idRightAreaStackView.replace("CreateTestPage.qml")
        }}
    ]
    property var tabsAdmin: [{name:"Родители",
            action:function(){
                idRightAreaStackView.replace("PeoplePage.qml", {"usersRole":UserRole.Parent})
            }},
        {name:"Воспитатели",
                    action:function(){
                        idRightAreaStackView.replace("PeoplePage.qml", {"usersRole":UserRole.Teacher})
                    }},
        {name:"Администраторы",
                    action:function(){
                        idRightAreaStackView.replace("PeoplePage.qml", {"usersRole":UserRole.Admin})
                    }},
        {name:"Дети",
                    action:function(){
                        idRightAreaStackView.replace("PeoplePage.qml", {"isShowUsers":false})
                    }}
    ]

    MyButton {
        id: idExitButton
        btnText: "Выйти"
        y: parent.height - height - 30
        x: parent.width - width - 30
        width: 150
        height: 50
        btnColor: Functions.mainColor

        z: 100

        onClicked: {
            console.log("log out button clicked")
            idRoot.logOut()
        }
    }

    Rectangle {
        id: idLeftAreaRectangle
        color: Functions.mainColor
        anchors {
            left: parent.left
            top: parent.top
            bottom: parent.bottom
        }
        width: {
            var w = parent.width * 0.3
            const minW = 150
            if(w < minW)
            {
                return minW
            }
            return w
        }

        Column {
            id: idUserDataColumn
            x: idConnectionStatusRow.x
            y: idConnectionStatusRow.y + idConnectionStatusRow.height + 30
            spacing: 15
            Text {
                id: idUserRoleText

                function getUserRoleText(role) {
                    switch(role)
                    {
                    case UserRole.Teacher: {
                        return "Воспитатель"
                    }
                    case UserRole.Parent : {
                        return "Родитель"
                    }
                    case UserRole.Admin: {
                        return "Администратор"
                    }
                    }
                }

                text: "<i>" + getUserRoleText(userInfo.userRole) + "</i>"
                color: "white"
                font {
                    pointSize: Functions.mainFontPointSize + 1
                    weight: Font.Medium
                }
            }
            Text {
                width: idLeftAreaRectangle.width - idUserDataColumn.x
                text: userInfo.userName
                font {
                    pointSize: Functions.mainFontPointSize + 2
                    weight: Font.DemiBold
                }
                style: Text.Raised
                wrapMode: Text.WordWrap
                color: "white"
            }
        }

        ListView {
            id: idTabsView
            anchors.horizontalCenter: parent.horizontalCenter
            x: idUserDataColumn.x
            y: idUserDataColumn.y + idUserDataColumn.height + 40
            width: parent.width * 0.9
            height: parent.height - y - 20
            spacing: 10

            delegate: Rectangle {
                width: idTabsView.width
                height: 40
                border.width: 2
                border.color: idTabMouseArea.containsMouse || index === idTabsView.currentIndex ? Qt.lighter(Functions.mainColor) : "transparent"
                radius: 2
                color: idTabMouseArea.containsPress ? Qt.darker(Functions.mainColor, 1.07) :"transparent"

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    x: 10
                    text: modelData.name
                    color: "white"
                    font.pointSize: Functions.mainFontPointSize
                }

                MouseArea {
                    id: idTabMouseArea
                    anchors.fill: parent
                    hoverEnabled: true

                    onClicked: {
                        idTabsView.currentIndex = index
                        switch(userInfo.userRole)
                        {
                        case UserRole.Teacher: {
                            tabsTeacher[index].action()
                            break
                        }
                        case UserRole.Parent : {
                            tabsParent[index].action()
                            break
                        }
                        case UserRole.Admin: {
                            tabsAdmin[index].action()
                            break
                        }
                        }
                    }
                }
            }

            Component.onCompleted: {
                switch(userInfo.userRole)
                {
                case UserRole.Teacher: {
                    model = tabsTeacher
                    tabsTeacher[0].action()
                    break
                }
                case UserRole.Parent : {
                    model = tabsParent
                    tabsParent[0].action()
                    break
                }
                case UserRole.Admin: {
                    model = tabsAdmin
                    tabsAdmin[0].action()
                    break
                }
                }
            }
        }
    }

    StackView {
        id: idRightAreaStackView
        anchors {
            left: idLeftAreaRectangle.right
            right: parent.right
            top: parent.top
            bottom: parent.bottom
        }

        Connections {
            target: idRightAreaStackView.currentItem
            ignoreUnknownSignals: true

            function onShowMessage(text) {
                idRoot.showMessage(text)
            }

            function onTestCreated() {
                idRightAreaStackView.replace("CreateTestPage.qml")
            }

            function onPassTest(index) {
                idRightAreaStackView.replace("PassingTest.qml", {"test":userInfo.getTest(index)})
            }

            function onTestFinished() {
                idRightAreaStackView.replace("TestsPage.qml")
            }

            function onAddEntity() {
                tabsAdmin[idTabsView.currentIndex].action()
            }
        }

        property int animationDuration: 300

        pushEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                easing.type: Easing.Linear
                from: 0
                to: 1
                duration: idRightAreaStackView.animationDuration
            }
        }

        replaceEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                easing.type: Easing.Linear
                from: 0
                to: 1
                duration: idRightAreaStackView.animationDuration
            }
        }

        replaceExit: Transition {
            PropertyAnimation {
                property: "opacity"
                easing.type: Easing.Linear
                from: 1
                to: 0
                duration: idRightAreaStackView.animationDuration
            }
        }

        pushExit: Transition {
            PropertyAnimation {
                property: "opacity"
                easing.type: Easing.Linear
                from: 1
                to: 0
                duration: idRightAreaStackView.animationDuration
            }
        }
        popEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 0
                to: 1
                duration: idRightAreaStackView.animationDuration
            }
        }
        popExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1
                to: 0
                duration: idRightAreaStackView.animationDuration
            }
        }
    }

    Component.onCompleted: {
        console.log("completed !", userInfo.userRole)
//        if(userInfo.userRole === UserRole.Admin)
//        {
//            connection.getAllUsers()
//        }
//        connection.getChildren()
    }
}
