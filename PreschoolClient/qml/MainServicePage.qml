import QtQuick 2.12
import "qrc:/Functions.js" as Functions

import preschool 1.0

Item {
    id: idRoot

    signal showMessage(string text)
    signal logOut()

    property var tabsParent: [{name:"Мой ребенок", action: function(){}},
        {name:"Тесты", action: function(){}},
        {name:"Оплата", action: function(){}}
    ]
    property var tabsTeacher: [{name:"Все дети",
        action: function(){}},
        {name:"Создать тесты", action: function(){}}
    ]
    property var tabsAdmin: [{name:"Пользователи",
            action:function(){}}
    ]

    MyButton {
        btnText: "Выйти"
        y: parent.height - height - 30
        x: parent.width - width - 30
        width: 150
        height: 50
        btnColor: Functions.mainColor

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
                    break
                }
                case UserRole.Parent : {
                    model = tabsParent
                    break
                }
                case UserRole.Admin: {
                    model = tabsAdmin
                    break
                }
                }
            }
        }
    }
}
