import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
pragma ComponentBehavior: Bound

Page {
    id: root

    property string errorMessage: ""
    property string titlePage: qsTr("Login")

    //  Signals for navigation
    signal closeRequested()
    signal helpClicked

    title: titlePage

    //  Main background layer
    background: Rectangle {
        color: Theme.cBg
        radius: 16
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        //  Title page
        TitleText {
            text: root.title
            Layout.fillWidth: true

            // Нижняя линия заголовка
            Rectangle {
                anchors.bottom: parent.bottom
                anchors.bottomMargin: -8
                width: parent.width
                height: 2
                color: Theme.cBorder
            }
        }

        // Отступ после разделителя
        Item { implicitHeight: 5 }

        StackView {
            id: mainStack
            Layout.preferredWidth: 400
            Layout.preferredHeight: 400
            Layout.alignment: Qt.AlignHCenter
            Layout.margins: 20

            clip: true

            initialItem: logIn

            Popup {
                id: errorPopup
                property string message: "" //  Свое свойство для текста ошибки или успеха

                //  Центрируем по горизонтали и поднимаем на 100 пикселей от низа
                x: (parent.width - width) / 2
                y: parent.height - 100
                width: Math.min(400, parent.width * 0.8)

                //  Настройки закрытия всплывающего окна
                modal: false //  Не блокирует взаимодействие с основным окном
                focus: false //  Не перехватывает ввод (Escape не сработает при false)

                visible: root.errorMessage !== ""

                // При закрытии окна автоматически очищаем текст ошибки в родителе
                onClosed: {
                    root.errorMessage = ""
                }

                //  Закроется, если нажать Esc (нужен focus: true) или кликнуть мимо
                closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

                Text {
                    anchors.centerIn: parent //  Центрируем надпись
                    text: root.errorMessage
                }
            }
        }
    }

    Component {
        id: logIn

        Page {
            background: Rectangle { color: "blue"; radius: 16 }
            //padding: 20

            ColumnLayout {
                //Layout.preferredWidth: implicitWidth
                anchors.centerIn: parent
                anchors.margins: 20
                spacing: 10

                //  Поле ввода логина
                TextField {
                    id: usernameField
                    Layout.preferredWidth: 200
                    Layout.alignment: Qt.AlignHCenter

                    color: activeFocus ? "black" : AuthController.existsByLogin(text) ? "black" : "red"
                    placeholderText: qsTr("Enter login")
                }

                //  Поле ввода пароля
                TextField {
                    id: passwordField
                    Layout.preferredWidth: 200
                    Layout.alignment: Qt.AlignHCenter

                    property bool error: false

                    color: activeFocus ? "black" : buttonIn.authenticationError ? "black" : "red"

                    placeholderText: qsTr("Enter password")
                    echoMode: TextField.Password    //  Скрывает вводимые символы точками
                }

                RowLayout {
                    Layout.preferredWidth: implicitWidth
                    Layout.alignment: Qt.AlignHCenter

                    Button {
                        id: buttonIn
                        property bool authenticationError: false
                        text: qsTr("Log in")
                        Layout.preferredWidth: 100
                        highlighted: true
                        onClicked: {
                            // Вызываем C++ функцию проверки
                            if (AuthController.authenticate(usernameField.text, passwordField.text)) {
                                errorAuthenticated: true
                            }
                        }
                    }

                    Button {
                        text: qsTr("Registration")
                        Layout.preferredWidth: 100
                        highlighted: true
                        //
                        // Используем встроенную системную иконку стрелки назад
                        icon.name: "go-next"
                        icon.height: 15
                        // Отображаем и иконку, и текст рядом
                        display: AbstractButton.TextBesideIcon

                        onClicked: {
                            mainStack.push(registration)
                            root.title = qsTr("Registration")
                        }
                    }
                }
            }
        }
    }

    Component {
        id: registration

        Page {
            background: Rectangle { color: "green"; radius: 16 }
            //padding: 20

            ColumnLayout {
                //Layout.preferredWidth: implicitWidth
                anchors.centerIn: parent
                anchors.margins: 20
                spacing: 10

                //  Поле ввода логина
                TextField {
                    id: usernameField
                    Layout.preferredWidth: 200
                    Layout.alignment: Qt.AlignHCenter

                    color: activeFocus ? "black" : AuthController.isUsernameUnique(text) ? "black" : "red"
                    placeholderText: qsTr("Come up with a username")
                }

                //  Поле ввода полного имени
                TextField {
                    id: fullNameField
                    Layout.preferredWidth: 200
                    Layout.alignment: Qt.AlignHCenter
                    color: "black"
                    placeholderText: qsTr("Enter your full name")
                }

                //  Поле ввода пароля
                TextField {
                    id: passwordField
                    Layout.preferredWidth: 200
                    Layout.alignment: Qt.AlignHCenter

                    property bool error: false

                    //color: activeFocus ? "black" : buttonIn.errorAuthenticated ? "black" : "red"

                    placeholderText: qsTr("Create a password")
                    echoMode: TextField.Password    //  Скрывает вводимые символы точками
                }

                RowLayout {
                    Layout.preferredWidth: implicitWidth
                    Layout.alignment: Qt.AlignHCenter

                    Button {
                        id: buttonIn
                        property bool errorAuthenticated: false
                        text: qsTr("Back")
                        Layout.preferredWidth: 100

                        // Используем встроенную системную иконку стрелки назад
                        icon.name: "go-previous"
                        icon.height: 15
                        // Отображаем и иконку, и текст рядом
                        display: AbstractButton.TextBesideIcon

                        //highlighted: true
                        onClicked: {
                            mainStack.pop()
                            root.title = qsTr("LogIn")
                        }
                    }

                    Button {
                        text: qsTr("Registration")
                        Layout.preferredWidth: 100
                        highlighted: true
                        onClicked: {
                            mainStack.push(registration)
                            root.title = qsTr("Registration")
                        }
                    }
                }
            }
        }
    }
}

// RowLayout {
//     Layout.fillWidth: true
//     Layout.preferredHeight: tableView.contentHeight
//     Layout.alignment: Qt.AlignVCenter
//     spacing: 0

//     VerticalHeaderView {
//         id: verticalHeader
//         Layout.preferredWidth: 100
//         Layout.preferredHeight: tableView.contentHeight
//         Layout.alignment: Qt.AlignVCenter
//         Layout.rightMargin: -28
//         syncView: tableView
//         clip: true

//         columnSpacing: 0
//         rowSpacing: 0
//     }

//     TableView {
//         id: tableView
//         Layout.fillWidth: true
//         Layout.preferredHeight: contentHeight
//         Layout.alignment: Qt.AlignVCenter
//         clip: true
//         boundsBehavior: Flickable.StopAtBounds
//         columnSpacing: 0
//         rowSpacing: 0

//         model: AuthController.registrationModel()

//         columnWidthProvider: function(column) {
//             return tableView.width;
//         }

//         delegate: Rectangle {
//             id: cell
//             implicitHeight: 25
//             //color: "transparent"
//             border.width: 1
//             border.color: "#dcdcdc"
//             //radius: 15

//             required property var display
//             required property var edit

//             TextInput {
//                 anchors.fill: parent
//                 anchors.margins: 8 // Задаем аккуратные внутренние отступы для текста
//                 verticalAlignment: TextInput.AlignVCenter

//                 text: cell.edit
//                 //font.pixelSize: 14
//                 color: "black"

//                 // Включаем выделение текста при фокусе, как в обычных полях
//                 selectByMouse: true
//             }
//         }
//     }
// }

// TextField {
//     anchors.fill: parent
//     verticalAlignment: TextInput.AlignVCenter
//     text: cell.display
//     anchors.margins: 8
// }

// ListView {
//     id: listView
//     Layout.fillWidth: true
//     //Layout.preferredHeight: tableView.contentHeight
//     Layout.alignment: Qt.AlignVCenter
//     model: AuthController.userRegistrationModel()

//     delegate: Rectangle {
//         id: cell
//         width: parent.width
//         height: 40
//         color: "lightgray"
//         border.color: "white"

//         required property var display
//         required property var edit

//         Text {
//             anchors.centerIn: parent
//             text: cell.edit
//             font.pixelSize: 16
//         }
//     }
// }
