import QtQuick 2.12
import "qrc:/Functions.js" as Functions

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
        text: "Создать тест"
    }
}
