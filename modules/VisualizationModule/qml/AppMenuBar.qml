import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs


MenuBar {
    id: root
    padding: 10

    // Привязываем ToolBar с учетом левого и правого маргина
    anchors.left: parent.left
    anchors.leftMargin: parent.anchors.leftMargin
    anchors.right: parent.right
    anchors.rightMargin: parent.anchors.rightMargin
    anchors.top: parent.top

    background: Rectangle {
        color: "#1e293b"
        radius: 6
    }

    delegate: MenuBarItem {
        id: menuBarItem
        implicitWidth: 100

        contentItem: Text {
            text: menuBarItem.text
            color: "#cbd5e1"
            font.pixelSize: 14
            font.weight: 500
            topPadding:  2; bottomPadding: 2
            leftPadding: 4; rightPadding:  4
            horizontalAlignment: Text.AlignHCenter
        }

        background: Rectangle {
            anchors.fill: parent
            color: menuBarItem.highlighted ? "#4a5f7d" : "#334155"
            anchors.margins: 5
            radius: 4
        }
    }

    // Палитра для чтения системных цветов ОС
    SystemPalette {
        id: sysPalette
        colorGroup: SystemPalette.Active
    }

    //  Пробрасываем сигналы
    signal openTriggered()


    //  Системное окно Справки
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
        title: qsTr("Settings")

        // Привязываем пункты меню к ранее созданным Action
        MenuItem { action: openAction }
        MenuItem { action: exitAction }
    }

    Menu {
        id: helpMenu
        title: qsTr("Help")

        MenuItem {
            text: qsTr("About ") + Qt.application.name
            onTriggered: aboutDialog.open()
        }
    }

}
