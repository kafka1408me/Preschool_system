import QtQuick 2.12
import QtQuick.Controls 2.12
import "qrc:/Functions.js" as Functions


//InputField {
//    id: idInputField
//    property alias descriptionText: idText.text
//    property alias inputFieldWith: idInputField.width
//    property alias echoMode: idInputField.echoMode

//    Text {
//        id: idText
//        x: - (contentWidth + 15)
//        anchors.verticalCenter: idInputField.verticalCenter
//        font.pointSize: idInputField.font.pointSize + 2
//        font.weight: Font.Medium
//        color: Functions.mainColor
//    }
//}

Row {
    spacing: 15
    property alias descriptionText: idText.text
    property alias inputFieldWith: idInputField.width
    property alias echoMode: idInputField.echoMode
    property alias textWidth: idText.width
    property alias descriptionContentWidth: idText.contentWidth
    property alias text: idInputField.text
    property alias descriptionTextColor: idText.color
    property alias fieldHeight: idInputField.rectHeight
    property alias fieldBackgroundBorderColor: idInputField.backgroundBorderColor
    property alias fieldBackgroundBorderFocusColor: idInputField.backgroundBorderFocusColor
    Text {
        id: idText
        anchors.verticalCenter: idInputField.verticalCenter
        font.pointSize: idInputField.font.pointSize + 2
        font.weight: Font.Medium
        color: Functions.mainColor
    }

    InputField {
        id: idInputField
    }
}

