import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

Window {
    id: idMainWindow
    width: Screen.width * 0.6
    height: width * 0.7
    visible: true

    StackView {
        id: idStackView
        anchors.fill: parent
        initialItem: "LoginPage.qml"
    }

}
