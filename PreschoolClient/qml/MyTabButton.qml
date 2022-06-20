import QtQuick 2.12
import QtQuick.Controls 2.12

import "qrc:/Functions.js" as Functions

TabButton {
    id: idRoot

    background: Rectangle {
        color: idRoot.checked ? Functions.mainColor : "white"
        implicitHeight: 30
    }
    contentItem: Text {
        id: idText
        text: idRoot.text
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
        color: idRoot.checked ? "white" : Functions.mainColor
        font.pointSize: Functions.mainFontPointSize
    }
}
