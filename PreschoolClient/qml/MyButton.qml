import QtQuick 2.12
import QtQuick.Controls 2.12
import "qrc:/Functions.js" as Functions

Button {
    id: idRoot
    property color btnColor: '#2a2c36'
    property alias btnText: btnTxt.text
    property alias btnRadius: buttonRect.radius
    property real btnBorderWidth: 0
    property color btnBorderColor: "transparent"

    property var btnPressedColor: Functions.makeMoreDarkColor(btnColor)

    background: Rectangle{
        id: buttonRect
        color: idRoot.down ? btnPressedColor : btnColor
        radius: height / 2
        border {
            width: idRoot.btnBorderWidth
            color: idRoot.btnBorderColor
        }

        Text{
            id: btnTxt
            anchors.centerIn: parent
            text: "Text"
            font.pointSize: 14
            color: idRoot.down ? Functions.whiteTextPressedColour : 'white'
        }
        z: 1
    }
}
