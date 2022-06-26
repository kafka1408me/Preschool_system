import QtQuick 2.12

import preschool 1.0
import "qrc:/Functions.js" as Functions

Item {
    id: idRoot
    property int usersRole: UserRole.Parent
    property bool isShowUsers: true

    property bool isTeacher: false

    Text {
        id: idTitleText
        anchors.horizontalCenter: parent.horizontalCenter
        y: 30
        color: "black"
        font {
            pointSize: 20
            weight: Font.Bold
        }
        text: {
            if(idRoot.isShowUsers)
            {
                switch(idRoot.usersRole)
                {
                case UserRole.Teacher: {
                    return "Воспитатели"
                }
                case UserRole.Parent : {
                    return "Родители"
                }
                case UserRole.Admin: {
                    return "Администраторы"
                }
                }
            }
            else
            {
                return "Дети"
            }
        }
    }

    ListView {
        id: idPeopleListView
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width * 0.9
        y: 120
        height: parent.height - y - 20
        spacing: 30
        model: idRoot.isShowUsers ? userInfo.usersModel : userInfo.childrenModel

        delegate: idRoot.isShowUsers ?idUserComponent : idChildComponent

        property real itemHeight: 90
    }

    Component{
        id: idUserComponent
        Item {
            id: idViewtem
            height: idPeopleListView.itemHeight
            width: idPeopleListView.width

            Image {
                id: idUserLogoImage
                source: "qrc:/images/user.png"
                height: parent.height * 0.85
                fillMode: Image.PreserveAspectFit
                asynchronous: true
            }
            Text {
                id: idNameText
                x: idUserLogoImage.x + idUserLogoImage.width + 30
                y: idUserLogoImage.y
                font {
                    pointSize: Functions.mainFontPointSize
                    weight: Font.DemiBold
                }
                text: model.name
                color: "black"
            }
            Text {
                x: idNameText.x
                anchors.bottom: idUserLogoImage.bottom
                font {
                    pointSize: Functions.mainFontPointSize
                    weight: Font.Medium
                }
                text: qsTr("id: %1").arg(model.id)
                color: "black"
            }
            Text {
                x: idNameText.x + 100
                anchors.bottom: idUserLogoImage.bottom
                font {
                    pointSize: Functions.mainFontPointSize
                    weight: Font.Medium
                }
                text: qsTr("login: %1").arg(model.login)
                color: "black"
            }

            Rectangle {
                anchors.bottom: parent.bottom
                width: parent.width
                height: 1
                color: "black"
            }

            MyButton {
                anchors.verticalCenter: parent.verticalCenter
                x: parent.width - width - 10
                width: 160
                height: 50
                btnColor: "#45c1f1"
                btnPressedColor: Qt.darker(btnColor, 1.06)
                btnText: "Редактировать"
                btnBorderWidth: 3
                btnBorderColor: "#44a4ec"
                btnRadius: 7
            }
        }
    }

    Component {
        id: idChildComponent
        Item {
            height: idPeopleListView.itemHeight
            width: idPeopleListView.width

            Image {
                id: idLogoImage
                source: model.gender === Gender.Male ? "qrc:/images/boy2.png" : "qrc:/images/girl2.png"
                height: parent.height * 0.85
                fillMode: Image.PreserveAspectFit
                asynchronous: true
            }
            Text {
                id: idNameText
                x: idLogoImage.x + idLogoImage.width + 30
                y: idLogoImage.y
                font {
                    pointSize: Functions.mainFontPointSize
                    weight: Font.DemiBold
                }
                text: model.name
                color: "black"
            }
            Text {
                id: idAgeText
                x: idNameText.x
                y: idNameText.y + idNameText.contentHeight + 5
                font {
                    pointSize: Functions.mainFontPointSize - 3
                    weight: Font.Normal
                }
                text: qsTr("Возраст: %1").arg(model.age)
                color: "black"
            }
            Text {
                visible: !isTeacher
                x: idNameText.x
                anchors.bottom: idLogoImage.bottom
                font {
                    pointSize: Functions.mainFontPointSize
                    weight: Font.Medium
                }
                text: qsTr("id: %1").arg(model.id)
                color: "black"
            }
            Text {
                x:  isTeacher? idNameText.x : idNameText.x + 100
                anchors.bottom: idLogoImage.bottom
                font {
                    pointSize: Functions.mainFontPointSize - 1
                    weight: Font.Medium
                }
                width: idChangeButton.x - x - 5
                text: isTeacher ? qsTr("Родитель: <i>%1</i>").arg(model.parentName) :
                                  qsTr("Родитель: <i>%1 (id: %2)</i>").arg(model.parentName).arg(model.parentId)
                color: "black"
                elide: Text.ElideRight
            }
            Rectangle {
                anchors.bottom: parent.bottom
                width: parent.width
                height: 1
                color: "black"
            }

            MyButton {
                id: idChangeButton
                anchors.verticalCenter: parent.verticalCenter
                x: parent.width - width - 10
                width: 160
                height: 50
                btnColor: "#45c1f1"
                btnPressedColor: Qt.darker(btnColor, 1.06)
                btnText: isTeacher ? "Информация" : "Редактировать"
                btnBorderWidth: 3
                btnBorderColor: "#44a4ec"
                btnRadius: 7
            }
        }
    }

    MyButtonWithPlus {
        visible: !isTeacher
        y: idExitButton.y
        x: idExitButton.x - idLeftAreaRectangle.width - width - 30
        btnText: "Добавить"
        btnRadius: 5
        height: 50
        width: 200

        onClicked: {
            var entityType
            if(isShowUsers)
            {
                if(usersRole === UserRole.Parent)
                {
                    entityType = "parent"
                }
                else if(usersRole === UserRole.Teacher)
                {
                    entityType = "teacher"
                }
                else if(usersRole === UserRole.Admin)
                {
                    entityType = "admin"
                }
            }
            else
            {
                entityType = "boy"
            }

            idRightAreaStackView.replace("CreateUserPage.qml", {"typeEntity":entityType})
        }
    }


    Component.onCompleted: {
        userInfo.setUsersRoleForModel(idRoot.usersRole)
    }
}
