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
        text: "Оплата за текущий месяц"
    }

    Column {
        x: parent.width * 0.07
        y: idTitleText.y + idTitleText.contentHeight + 40
        spacing: 20
        Text {
            id: idDayPriceText
            text: "День: 400 р"
            color: "black"
            font {
                pointSize: 17
                weight: Font.DemiBold
            }
        }
        Text {
            text: "Посещений: 4"
            color: "black"
            font: idDayPriceText.font
        }
        Text {
            text: "<strong>Итоговая сумма:</strong> 1600 р"
            color: "black"
            font.pointSize: idDayPriceText.font.pointSize + 1
        }
    }
}
