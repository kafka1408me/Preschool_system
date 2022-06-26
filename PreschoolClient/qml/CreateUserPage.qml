import QtQuick 2.12
import QtQuick.Controls 2.12

import preschool 1.0
import "qrc:/Functions.js" as Functions

Item {
    id: idRoot
    property string typeEntity: "admin"
    property bool isAddOperation: true

    readonly property bool isChild: typeEntity == "boy" || typeEntity === "girl"

    signal showMessage(string text)

    signal addEntity()

    TitleText {
        id: idTitleText
        text: {
            var txt = isAddOperation ? "Добавление ": "Редактирование "
            if(typeEntity == "admin")
            {
                txt += "администратора"
            }
            else if(typeEntity == "parent")
            {
                txt += "родителя"
            }
            else if(typeEntity == "teacher")
            {
                txt += "воспитетеля"
            }
            else
            {
                txt += "ребенка"
            }
            return txt
        }
    }

    Image {
        id: idLogoImage
        source: isChild ?
                    (typeEntity == "boy" ? "qrc:/images/boy2.png" : "qrc:/images/girl2.png") :
                    "qrc:/images/user.png"
        height: 100
        width: 100
        x: parent.width - width - 40
        y: idTitleText.y + idTitleText.contentHeight + 40
        //   fillMode: Image.PreserveAspectFit
        asynchronous: true

        Rectangle {
            id: idImageBorderRect
            anchors.centerIn: parent
            color: "transparent"
            width: parent.width + border.width*2
            height: width
            border.width: 2
            border.color: "#c4c4c4"
        }
    }

    ListView {
        id: idFieldsView
        anchors {
            horizontalCenter: parent.horizontalCenter
        }
        width: parent.width * 0.7
        height: parent.height - y - 20
        model: [
            {name: "Имя", visible: true, delegate: "input", key: "name"},
            {name: "Возраст", visible: isChild, delegate: "input", key: "age"},
            {name: "Логин", visible: !isChild, delegate: "input", key: "login"},
            {name: "Пароль", visible: !isChild, delegate: "input", key: "password"},
            {name: "Воспитатель", visible: isChild, delegate: "combobox", key: "teacher"},
            {name: "Родитель", visible: isChild, delegate: "combobox", key: "parent"},
            {name: "Пол", visible: isChild, delegate: "combobox", key: "gender"},
        ]
        y: idLogoImage.y

        function isFilled() {
            for(var i = 0; i < count; ++i)
            {
                if(!idFieldsView.itemAtIndex(i).isFilled())
                {
                    return false
                }
            }
            return true
        }

        function getValue() {
            var value = {}
            for(var i = 0; i < count; ++i)
            {
                if(idFieldsView.itemAtIndex(i).visible)
                {
                    value[idFieldsView.itemAtIndex(i).name] = getValueForItem(i)
                }
            }
            return value
        }

        function getValueForItem(index) {
            return idFieldsView.itemAtIndex(index).value()
        }

        function clearItemsValues() {
            for(var i = 0; i < count; ++i)
            {
                idFieldsView.itemAtIndex(i).clear()
            }
        }

        delegate: Item {
            id: idEntityItem
            width: parent.width
            height: modelData.visible ? 80 : 0
            visible: modelData.visible
            property string name: modelData.key
            function isFilled() {
                if(!visible)
                {
                    return true
                }

                return idLoader.item.isFilled()
            }

            function value() {
                return idLoader.item.value()
            }

            function clear() {
                if(!visible)
                {
                    return
                }
                idLoader.item.clear()
            }

            Loader {
                id: idLoader
                parent: idEntityItem
                anchors.fill: parent
                active: modelData.visible
                sourceComponent: modelData.delegate === "input" ? idInputFieldComp: idComboboxComp
            }

            Component {
                id: idInputFieldComp
                InputFieldWithDescription {
                    anchors.verticalCenter: parent.verticalCenter
                    descriptionText: modelData.name
                    textWidth: 150
                    fieldHeight: 50
                    inputFieldWith: 250
                    function isFilled() {
                        return text.length > 0
                    }
                    function value() {
                        return text
                    }

                    function clear() {

                    }
                }
            }

            Component {
                id: idComboboxComp
                Row {
                    spacing: 15
                    function isFilled() {
                        return idCmbx.currentText.length > 0
                    }

                    function value() {
                        var value = idCmbx.currentValue
                        if(modelData.name === "Воспитатель" || modelData.name === "Родитель")
                        {
                            var users = userInfo.getAllUsers()
                            for(var i = 0; i < users.length; ++i)
                            {
                                if(users[i].name === idCmbx.currentText)
                                {
                                    return users[i].id
                                }
                            }
                        }
                        else
                        {
                            if(idCmbx.currentText === "Мужской")
                            {
                                return 1
                            }
                            else
                            {
                                return 2
                            }
                        }
                    }

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        width: 150
                        text: modelData.name
                        font.pointSize: 16
                        font.weight: Font.Medium
                        color: Functions.mainColor
                    }
                    ComboBox {
                        id: idCmbx
                        anchors.verticalCenter: parent.verticalCenter
                        height: 50
                        width: 250

                        ListModel{
                            id: idCmbxModel
                        }
                        model: idCmbxModel

                        onCurrentTextChanged: {
                            console.log("name", modelData.name)
                            if (modelData.name === "Пол")
                            {
                                if(currentText === "Мужской")
                                {
                                    idRoot.typeEntity = "boy"
                                }
                                else
                                {
                                    idRoot.typeEntity = "girl"
                                }
                            }
                        }

                        Component.onCompleted: {
                            if(modelData.name === "Воспитатель")
                            {
                                var users = userInfo.getAllUsers()
                                for(var i = 0; i < users.length; ++i)
                                {
                                    if(users[i].role === UserRole.Teacher)
                                    {
                                        idCmbxModel.append({text: users[i].name})
                                    }
                                }
                            }
                            else if(modelData.name === "Родитель")
                            {
                                users = userInfo.getAllUsers()
                                var children = userInfo.getAllChildren()
                                for(i = 0; i < users.length; ++i)
                                {
                                    if(users[i].role === UserRole.Parent)
                                    {
                                        var hasChild = false
                                        for(var j = 0; j < children.length; ++j)
                                        {
                                            if(children[j].parentId === users[i].id)
                                            {
                                                hasChild = true
                                            }
                                        }
                                        if(!hasChild)
                                        {
                                            idCmbxModel.append({text: users[i].name})
                                        }
                                    }
                                }
                            }
                            else
                            {
                                idCmbxModel.append({text:"Мужской"})
                                idCmbxModel.append({text:"Женский"})
                                currentIndex = 0
                            }
                        }
                    }
                }
            }
        }
    }

    onAddEntity: {
        idFieldsView
    }

    MyButtonWithPlus {
        id: idAddButton
        y: idExitButton.y
        x: idExitButton.x - idLeftAreaRectangle.width - width - 30
        btnText: "Завершить"
        btnRadius: 5
        height: 50
        width: 220

        onClicked: {
            if(idFieldsView.isFilled())
            {
                if(isAddOperation)
                {
                    var value = idFieldsView.getValue()
                    if(isChild)
                    {
                        var name = value.name
                        var age = value.age
                        var parent = value.parent
                        var teacher = value.teacher
                        var gender = value.gender
                        console.log("teacher", teacher)
                        connection.addChild(name, age, gender, parent, teacher)
                    }
                    else
                    {
                        name = value.name
                        var login = value.login
                        var password = value.password
                        var role
                        if(typeEntity === "parent")
                        {
                            role = UserRole.Parent
                        }
                        else if(typeEntity === "teacher")
                        {
                            role = UserRole.Teacher
                        }
                        else
                        {
                            role = UserRole.Admin
                        }

                        connection.addUser(name, login, password, role)
                    }
                    idRoot.addEntity()
                }
            }
            else
            {
                idRoot.showMessage("Заполните все поля")
            }
        }
    }
}
