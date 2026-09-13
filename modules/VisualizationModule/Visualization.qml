import QtQuick          //  Базовые элементы (Rectangle, Image, Text)
import QtQuick.Controls //  Стандартные элементы (Window, Button, Popup, Menu, SplitView)

pragma ComponentBehavior: Bound

ApplicationWindow {
    id: window

    //readonly property Drawer mainMenu: menu

    visible: true

    width: 1200
    height: 700

    minimumWidth: 500
    minimumHeight: 500

    menuBar: AppMenuBar {
        id: appMenuBar

        width: parent.width
        anchors.top: parent.top
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        anchors.topMargin: 0
        anchors.bottomMargin: 5
    }

    Drawer {
        id: menu
        width: 250
        height: parent.height

        ListView {
            anchors.fill: parent
            model: ["Login", "Dashboard", "Inspection", "Statistics", "Operator", "OperatorsTable", "Settings"]    //, "Dataset", "Settings"

            delegate: ItemDelegate {
                required property string modelData

                text: modelData
                onClicked: {
                    stackView.push("qml/pages/" + modelData + "Page.qml")
                    menu.close()
                }
            }
        }
    }

    // AuthController {
    //     id: authController
    //     onAuthSuccess: {
    //         //  При успешном входе полностью заменяем экран входа на главный экран
    //         mainStack.replace (operatorPage)
    //     }
    //     onAuthFailed: (error) => {
    //         mainStack.currentItem.errorMessage = error
    //     }
    // }

    // StackView {
    //     id: mainStack
    //     anchors.fill: parent
    //     //  По умолчанию загружается стартовая страница (экран входа)
    //     initialItem: loginPage
    // }

    // --- КОМПОНЕНТЫ ЭКРАНОВ ---

    // 1. Стартовая страница (Вход / Регистрация)
    // Component {
    //     id: loginPage
    //     LoginPage {
    //         // Передаем наш сервис в кастомный виджет
    //         authBackend: authController

    //         onRegistrationClicked: {
    //             // При клике на регистрацию накладываем экран сверху
    //             mainStack.push(registrationPage)
    //         }
    //         onHelpClicked: {
    //             // При клике на справку накладываем экран справки
    //             mainStack.push(helpPage)
    //         }
    //     }
    // }

    // 2. Страница регистрации
    // Component {
    //     id: registrationPage
    //     Page {
    //         title: "Регистрация"
    //         Column {
    //             anchors.centerIn: parent
    //             spacing: 15
    //             Text { text: "Экран регистрации нового пользователя"; font.pixelSize: 20; color: "white" }
    //             Button {
    //                 text: "Назад"
    //                 onClicked: mainStack.pop() // Возвращаемся на экран входа
    //             }
    //         }
    //         background: Rectangle { color: "#111827" }
    //     }
    // }

    // 3. Страница Справки
    // Component {
    //     id: helpPage
    //     Page {
    //         title: "Справка"
    //         Column {
    //             anchors.centerIn: parent
    //             spacing: 15
    //             Text { text: "Здесь находится инструкция и справка"; font.pixelSize: 20; color: "white" }
    //             Button {
    //                 text: "Назад"
    //                 onClicked: mainStack.pop()
    //             }
    //         }
    //         background: Rectangle { color: "#111827" }
    //     }
    // }

    // Component {
    //     id: operatorPage
    //     OperatorPage {

    //     }
    // }

    // 4. Главная страница приложения (Доступна ТОЛЬКО после входа)
    // Component {
    //     id: mainAppPage
    //     Page {
    //         title: "Главная"
    //         Column {
    //             anchors.centerIn: parent
    //             spacing: 20
    //             Text { text: "Добро пожаловать в систему!"; font.pixelSize: 24; color: "white" }
    //             Button {
    //                 text: "Выйти из системы"
    //                 // При выходе возвращаем экран логина
    //                 onClicked: mainStack.replace(loginPage)
    //             }
    //         }
    //         background: Rectangle { color: "#065F46" } // Наш зеленый цвет из прошлых шагов
    //     }
    // }

    Item {

        id: customHeaderContainer

        width: parent.width
        height: appToolBar.implicitHeight + anchors.topMargin + anchors.bottomMargin

        anchors.top: parent.top
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        anchors.topMargin: 0
        anchors.bottomMargin: 5

        AppToolBar {
            id: appToolBar
            onMenuTriggered: menu.open()
        }

    }

    StackView {
        id: stackView

        anchors.top: customHeaderContainer.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        anchors.leftMargin: 5
        anchors.rightMargin: 5
        anchors.topMargin: 5
        anchors.bottomMargin: 15

        initialItem: "qml/pages/SettingsPage.qml"
    }

}
