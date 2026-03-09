import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs


ApplicationWindow {
    id: window
    width: 640; height: 480
    visible: true


    //  Подхватываем значение из C++ и сохраняем в типизированное св-во
    readonly property string appName: Qt.application.name


    //  1. Описываем действия (Actions), чтобы не дублировать код
    Action {
        id: openAction
        text: "&Открыть..."
        shortcut: StandardKey.Open
        onTriggered: console.log("Нажали 'Открыть'")
    }

    Action {
        id: exitAction
        text: "&Выход"
        shortcut: "Ctrl+Q"
        onTriggered: Qt.quit()  //  Стандартная ф-ция выхода
    }

    // 2. Добавляем саму полосу меню
    menuBar: MenuBar {

        Menu {
            title: "&Файл"
            MenuItem { action: openAction }
            MenuItem { action: exitAction }
        }

        Menu {
            title: "&Правка"
            MenuItem { text: "Копировать"; enabled: false }    //  Пример неактивного пункта
            MenuItem { text: "Вставить" }
        }

        Menu {
            id: helpMenu
            title: "&Помощь"
            MenuItem {
                text: "О программе"
                onTriggered: aboutDialog.open()
            }
        }
    }

    MessageDialog {
        id: aboutDialog
        title: "О программе"
        text: window.appName + "\nВерсия 1.0.0\nРазработано на Qt 6"
        buttons: MessageDialog.Ok
    }


    SplitView {
        id: splitView
        anchors.fill: parent
        anchors.margins: 5
        orientation: Qt.Horizontal

        //  боковое меню
        Rectangle {
            id: sideBar
            //color: "#2c3e50"
            color: Qt.rgba(44/255, 62/255, 80/255, 0.9)
            radius: 10

            SplitView.preferredWidth: 150
            SplitView.minimumWidth: 100
            SplitView.maximumWidth: 300

            ButtonSideBar {
                //  передаем ссылки на объекты из viewer
                targetHandler: viewer.handler
                targetPopup: viewer.popup
            }
        }


        //  Центр окна (ваш контент)
        ColumnLayout {
            SplitView.fillWidth: true

            ImageView {
                id: viewer
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }


    /*ListView {
        anchors.fill: parent
        model: myGlobalModel    //  ссылаемся на модель по имени, к-рое дидим в main.cpp
        spacing: 5

        //  DELEGATE: Как выглядит одна строка
        delegate: Rectangle {
            width: parent.width
            height: 40
            color: "lightgray"
            border.color: "darkgray"

            Text {
                anchors.centerIn: parent
                text: name  //  используем роль 'name' из нашей C++ модели
                color: "blue"
                font.pixelSize: 16
            }
        }
    }*/
}
