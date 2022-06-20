import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4

import "qrc:/Functions.js" as Functions
import preschool 1.0

Item {
    id: idRoot

    property var currentDay: new Date()

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
            id: idChildDelegateItem
            width: idListView.width
            height: idListView.height

            property var visitDays: model.visitDays

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

//            Text {
//                width: idNameChildColumn.width
//                x: idNameChildColumn.x
//                y: idCalendar.y - contentHeight - 4
//                font {
//                    pointSize: Functions.mainFontPointSize + 1
//                    weight: Font.Bold
//                }
//                text: "Посещаемость"
//            }

            TabBar {
                width: idNameChildColumn.width
                x: idNameChildColumn.x
                y: idCalendar.y - contentHeight - 4
                MyTabButton{
                    text: "Посещаемость"
                }
                MyTabButton {
                    text: "Оценки"
                }
            }

            Calendar {
                id: idCalendar
                x: idNameChildColumn.x
                anchors.top: idLogoImage.bottom
                width: idNameChildColumn.width
                height: 300
                enabled: false

                // Стилизуем Календарь
                style: CalendarStyle {
                    dayDelegate: Rectangle {
                        id: dateItem
                        anchors.fill: parent

                        color: {
                            if(isVisitedDay)
                            {
                                return "#76f776"
                            }
                            if(isCurrentDay)
                            {
                                return Functions.mainColor
                            }
                            return "transparent"
                        }

                        property bool isVisitedDay: isCurrentMonth && idChildDelegateItem.visitDays.includes(styleData.date.getDate())
                        property bool isCurrentDay: idRoot.currentDay.getDate() === styleData.date.getDate() && styleData.visibleMonth
                        property bool isCurrentMonth: styleData.visibleMonth

                        // Помещаем Label для отображения числа

                        Label {
                            id: dayDelegateText
                            text: styleData.date.getDate() // Устанавливаем число в текущий квадрат
                            anchors.centerIn: parent
                            horizontalAlignment: Text.AlignRight
                            font.pointSize: 12

                            // Установка цвета
                            color: dateItem.isCurrentDay || dateItem.isVisitedDay ? "white" : "#51b9eb"
                        }
                    }
                }
            }
        }
    }
}
