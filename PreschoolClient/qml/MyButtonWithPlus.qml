import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import "qrc:/Functions.js" as Functions

Button {
    id: idRoot
    property color btnColor: "#ececec"
    property alias btnText: btnTxt.text
    property alias btnRadius: buttonRect.radius
    property real btnBorderWidth: 2
    property color btnBorderColor: Qt.darker(btnColor, 1.17)

    property color btnPressedColor: Qt.darker(btnColor, 1.12)

    background: Rectangle{
        id: buttonRect
        color: idRoot.down ? btnPressedColor : btnColor
        radius: height / 2
        border {
            width: idRoot.btnBorderWidth
            color: idRoot.btnBorderColor
        }

        Image{
            id: idButtonImage
            anchors.verticalCenter: parent.verticalCenter
            x: btnRadius + 2
            visible: false
            source: "qrc:/images/plus.png"
            height: 32
            fillMode: Image.PreserveAspectFit
        }
        ColorOverlay {
            id: idColorOverlay
            anchors.verticalCenter: parent.verticalCenter
            anchors.fill: idButtonImage
            source: idButtonImage
            color: Qt.darker(idRoot.btnColor, 1.5)
        }
        Text{
            id: btnTxt
            anchors.verticalCenter: parent.verticalCenter
            x: idButtonImage.x + idButtonImage.width + 10
            text: "Text"
            font.pointSize: Functions.mainFontPointSize
            color: Qt.darker(idRoot.btnColor, 1.8)
        }

    }
}
