import QtQuick 2.12
import "qrc:/Functions.js" as Functions


InputField {
    id: idInputField
    property alias descriptionText: idText.text
    property alias inputFieldWith: idInputField.width
    property alias echoMode: idInputField.echoMode

    Text {
        id: idText
        x: - (contentWidth + 15)
        anchors.verticalCenter: idInputField.verticalCenter
        font.pointSize: idInputField.font.pointSize + 2
        font.weight: Font.Medium
        color: Functions.mainColor
    }
}

