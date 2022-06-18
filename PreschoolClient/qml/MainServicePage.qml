import QtQuick 2.12
import "qrc:/Functions.js" as Functions

import preschool 1.0

Item {
    id: idRoot

    Rectangle {
        color: Functions.mainColor
        anchors {
            left: parent.left
            top: parent.top
            bottom: parent.bottom
        }
        width: {
            var w = parent.width * 0.3
            const minW = 150
            if(w < minW)
            {
                return minW
            }
            return w
        }

        Text {
            id: idUserRoleText
            x: idConnectionStatusRow.x
            y: idConnectionStatusRow.y + idConnectionStatusRow.height + 30
            text: {
                switch(3)
                {
                case UserRole.Teacher: {
                    return "Воспитатель"
                }
                case UserRole.Parent : {
                    return "Родитель"
                }
                case UserRole.Admin: {
                    return "Администратор"
                }
                }
            }
            color: "white"
            font {
                pointSize: Functions.mainFontPointSize + 1
                weight: Font.Medium
            }
        }
    }
}
