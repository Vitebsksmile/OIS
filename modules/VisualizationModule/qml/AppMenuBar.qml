import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs


MenuBar {

    //  Пробрасываем сигналы
    signal openTriggered()


    //  Системное окно с информацией о программе
    MessageDialog {

        id: aboutDialog

        title: qsTr("About ") + Qt.application.name
        text: Qt.application.name + "\nВерсия v" + Qt.application.version + "\nРазработано на Qt 6"
        buttons: MessageDialog.Ok

    }


    //  1. Описание логики действий (Actions) - это "мозг" кнопок и меню
    Action {

        id: openAction

        text: qsTr("&Open...")         //  Символ '&' позволяет нажать Alt+О для активации
        shortcut: StandardKey.Open  //  Автоматически подхватывает Ctrl+O (Win) или Cmd+O (Mac)
        onTriggered: console.log(`Нажали ${openAction.text}`)

    }


    Action {

        id: exitAction

        text: qsTr("&Exit")
        shortcut: "Ctrl+Q"  //  Быстрое сочетание клавиш
        onTriggered: console.log(`Нажали ${exitAction.text}`) | Qt.quit()  //  Стандартная ф-ция выхода

    }


    // 2. Добавляем элементы меню
    Menu {

        id: fileMenu
        title: qsTr("&File")

        // Привязываем пункты меню к ранее созданным Action
        MenuItem { action: openAction }
        MenuItem { action: exitAction }

    }


    Menu {

        id: editMenu
        title: qsTr("&Edit")

        MenuItem { text: qsTr("Copy"); enabled: false }    //  Пример неактивного пункта
        MenuItem { text: qsTr("Paste") }

    }


    Menu {

        id: helpMenu
        title: qsTr("&Help")

        MenuItem {
            text: qsTr("About ") + Qt.application.name
            onTriggered: aboutDialog.open()
        }

    }

}
