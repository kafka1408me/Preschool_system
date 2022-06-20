import QtQuick 2.12
import QtQuick.Controls 2.12
import "qrc:/Functions.js" as Functions

TextField {
    id: lineEdit

    property color backgroundBorderColor: "#adbff3"
    property color backgroundBorderFocusColor: Functions.mainColor
    property alias backgroundColor: backgroundRectangle.color
    property alias rectHeight: backgroundRectangle.implicitHeight

    font.pointSize: 14

    background: Rectangle {
        id: backgroundRectangle

        implicitHeight: 56
        height: implicitHeight

        radius: 4

        border {
            color: lineEdit.activeFocus ? backgroundBorderFocusColor : backgroundBorderColor
            width: 2
        }
        color: "white"
    }
}
