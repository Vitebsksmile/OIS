import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
pragma ComponentBehavior: Bound

Page {
    id: root

    property string errorTitle: ""
    property string errorDetails: ""
    property string titlePage: qsTr("Login")
    property int textFieldWidth: 300
    property int componentSpacing: 10

    //  Signals for navigation
    signal closeRequested()
    signal helpClicked

    title: titlePage

    //  Main background layer
    background: Rectangle { color: Theme.cBg; radius: 12 }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: root.componentSpacing

        //  Title page
        TitleText {
            Layout.fillWidth: true
            text: root.title

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

            ErrorTitlePopup {
                id: errorTitle
                errorTitle: root.errorTitle
                errorDetails: root.errorDetails
            }
        }
    }

    Component {
        id: logIn

        Page {
            background: Rectangle { color: Theme.cBg; radius: 16 }
            //padding: 20

            ColumnLayout {
                //Layout.preferredWidth: implicitWidth
                anchors.centerIn: parent
                anchors.margins: 20
                spacing: root.componentSpacing

                //  Поле ввода логина
                OISTextField {
                    id: usernameField
                    Layout.preferredWidth: root.textFieldWidth
                    Layout.alignment: Qt.AlignHCenter

                    color: activeFocus ? "black" : AuthController.existsByLogin(text) ? "black" : "red"
                    font.family: Theme.fFamily
                    font.pixelSize: Theme.fSizeNormal
                    font.bold: Theme.fBoldNormal

                    placeholderText: qsTr("Enter login")
                }

                //  Поле ввода пароля
                OISTextField {
                    id: passwordField
                    Layout.preferredWidth: root.textFieldWidth
                    Layout.alignment: Qt.AlignHCenter

                    property bool error: false

                    color: activeFocus ? "black" : buttonIn.authenticationError ? "black" : "red"
                    font.family: Theme.fFamily
                    font.pixelSize: Theme.fSizeNormal
                    font.bold: Theme.fBoldNormal

                    placeholderText: qsTr("Enter password")
                    echoMode: TextField.Password    //  Скрывает вводимые символы точками
                }

                RowLayout {
                    //Layout.preferredWidth: implicitWidth
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter
                    spacing: root.componentSpacing

                    Button {
                        id: buttonIn
                        Layout.preferredWidth: (root.textFieldWidth - root.spacing) / 2
                        property bool authenticationError: false

                        text: qsTr("Log in")
                        //highlighted: true

                        onClicked: {
                            // Вызываем C++ функцию проверки
                            if (AuthController.login(usernameField.text, passwordField.text)) {
                                authenticationError: true
                            }
                        }
                    }

                    Button {
                        Layout.preferredWidth: (root.textFieldWidth - root.spacing) / 2

                        text: qsTr("Registration")
                        //highlighted: true

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
            background: Rectangle { color: Theme.cBg; radius: 16 }
            //padding: 20

            ColumnLayout {
                anchors.centerIn: parent
                anchors.margins: 20
                spacing: root.componentSpacing

                //  Поле ввода логина
                OISTextField {
                    id: usernameField
                    Layout.preferredWidth: root.textFieldWidth
                    Layout.alignment: Qt.AlignHCenter

                    color: activeFocus ? "black" : AuthController.isUsernameUnique(text) ? "black" : "red"
                    font.family: Theme.fFamily
                    font.pixelSize: Theme.fSizeNormal
                    font.bold: Theme.fBoldNormal

                    placeholderText: qsTr("Come up with a username")
                }

                //  Поле ввода полного имени
                OISTextField {
                    id: fullNameField
                    Layout.preferredWidth: root.textFieldWidth
                    Layout.alignment: Qt.AlignHCenter
                    color: "black"
                    font.family: Theme.fFamily
                    font.pixelSize: Theme.fSizeNormal
                    font.bold: Theme.fBoldNormal

                    placeholderText: qsTr("Enter your full name")
                }

                //  Поле ввода пароля
                OISTextField {
                    id: passwordField
                    Layout.preferredWidth: root.textFieldWidth
                    Layout.alignment: Qt.AlignHCenter

                    property bool error: false

                    font.family: Theme.fFamily
                    font.pixelSize: Theme.fSizeNormal
                    font.bold: Theme.fBoldNormal

                    placeholderText: qsTr("Create a password")
                    echoMode: TextField.Password    //  Скрывает вводимые символы точками
                }

                RowLayout {
                    Layout.preferredWidth: implicitWidth
                    Layout.alignment: Qt.AlignHCenter
                    spacing: root.componentSpacing

                    Button {
                        id: buttonIn
                        Layout.preferredWidth: (root.textFieldWidth - root.spacing) / 2

                        text: qsTr("Back")
                        //highlighted: true

                        // Используем встроенную системную иконку стрелки назад
                        icon.name: "go-previous"
                        icon.height: 15
                        // Отображаем и иконку, и текст рядом
                        display: AbstractButton.TextBesideIcon

                        onClicked: {
                            mainStack.pop()
                            root.title = qsTr("LogIn")
                        }
                    }

                    Button {
                        Layout.preferredWidth: (root.textFieldWidth - root.spacing) / 2

                        text: qsTr("Sign up")
                        //highlighted: true

                        onClicked: {
                            //  вызов ф-ции регистрации
                            if (AuthController.registerUser(usernameField.text,
                                                            fullNameField.text,
                                                            passwordField.text)) {
                                //  обработка результата:
                                //  - если успех - уведобление об успехе и переход на страницу входа
                                mainStack.push(logIn)
                                root.title = qsTr("LogIn")
                            } else {
                                //  обработка результата:
                                //  - если ошибка - уведомление об ошибке
                            }
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
