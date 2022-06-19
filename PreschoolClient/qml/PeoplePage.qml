import QtQuick 2.12

import preschool 1.0
import "qrc:/Functions.js" as Functions

Item {
    id: idRoot
    property int usersRole: UserRole.Parent

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
    }

    ListView {
        id: idPeopleListView
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width * 0.9
        y: 120
        height: parent.height - y - 20
        spacing: 30
        model: userInfo.usersModel

        delegate: idUserComponent
    }

    Component{
        id: idUserComponent
        Rectangle {
            height: 90
            width: idPeopleListView.width

            Image {
                id: idUserLogoImage
                source: "qrc:/images/user.png"
                height: parent.height * 0.85
                fillMode: Image.PreserveAspectFit
                asynchronous: true
            }
            Text {
                id: idIserNameText
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
                x: idIserNameText.x
                anchors.bottom: idUserLogoImage.bottom
                font {
                    pointSize: Functions.mainFontPointSize
                    weight: Font.Medium
                }
                text: qsTr("id: %1").arg(model.id)
                color: "black"
            }
            Text {
                x: idIserNameText.x + 100
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
                x: parent.width - width - 20
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

    Component.onCompleted: {
        userInfo.setUsersRoleForModel(idRoot.usersRole)
    }
}
