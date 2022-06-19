import QtQuick 2.12

import "qrc:/Functions.js" as Functions
import preschool 1.0

Item {

    Text {
        id: idTitleText
        anchors.horizontalCenter: parent.horizontalCenter
        y: 30
        color: "black"
        font {
            pointSize: 20
            weight: Font.Bold
        }
        text: "Мой ребенок"
    }

    ListView {
        id: idListView

        anchors.horizontalCenter: parent.horizontalCenter
        y: idTitleText.y + idTitleText.contentHeight
        width: parent.width * 0.9
        height: parent.height - y - 30

        model: userInfo.childrenModel

        delegate: Item {
            width: idListView.width
            height: idListView.height


            Column {
                id: idNameChildColumn
                spacing: 15
                x: idLogoImage.x - parent.width * 0.5

                y: idLogoImage.y
                width: idLogoImage.x - x - 20
                Text {
                    id: idChildNameText
                    width: idNameChildColumn.width
                    height: contentHeight
                    font {
                        pointSize: 18
                        weight: Font.Bold
                    }
                    text: model.name
                    wrapMode: Text.WordWrap
                }

                Text {
                    width: idNameChildColumn.width
                    font {
                        pointSize: Functions.mainFontPointSize
                        weight: Font.Normal
                    }
                    text: qsTr("Возраст: %1").arg(model.age)
                    wrapMode: Text.WordWrap
                }

                Text {
                    width: idNameChildColumn.width
                    font {
                        pointSize: Functions.mainFontPointSize
                        weight: Font.Normal
                    }
                    text: {
                        return qsTr("Воспитатель: %1").arg(model.teacherName)
                    }
                    wrapMode: Text.WordWrap
                }
            }

            Image {
                id: idLogoImage
                source: model.gender === Gender.Male ? "qrc:/images/boy2.png" : "qrc:/images/girl2.png"
                height: parent.height * 0.3
                x: parent.width - width - 40
                y: idTitleText.y + idTitleText.contentHeight + 30
                fillMode: Image.PreserveAspectFit
                asynchronous: true

                Rectangle {
                    id: idImageBorderRect
                    anchors.centerIn: parent
                    color: "transparent"
                    width: parent.width + border.width*2
                    height: width
                    border.width: 2
                    border.color: "#c4c4c4"
                }
            }
        }
    }
}
