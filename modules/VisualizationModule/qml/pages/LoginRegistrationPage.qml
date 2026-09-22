import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
pragma ComponentBehavior: Bound

Page {
    id: root

    property string errorMessage: ""
    property string titlePage: "Login"

    //  Signals for navigation
    signal closeRequested()
    signal helpClicked

    Connections {
        target: AuthController
        function onAuthFailed() {
            //usernameField.color: "red"
        }
    }

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

        Rectangle {
            Layout.preferredWidth: 400
            Layout.preferredHeight: 400
            Layout.alignment: Qt.AlignHCenter
            radius: 16
            Layout.margins: 20
            color: "green"

            StackView {
                id: mainStack
                // Layout.preferredWidth: 400
                // Layout.preferredHeight: 400
                // Layout.alignment: Qt.AlignHCenter
                anchors.fill: parent
                anchors.margins: 20
                clip: true

                initialItem: logIn
            }
        }
    }

    Component {
        id: logIn

        Page {
            background: Rectangle { color: "blue"; radius: 16 }
            anchors.margins: 20

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

                    property bool error: false

                    background: Rectangle {
                        border.color: usernameField.error ? Theme.cBorderDanger : Theme.cBorder
                    }

                    color: "black"
                    placeholderText: "Введите логин"
                    onEditingFinished: {
                        if (!AuthController.identification(text)) {
                            error = true
                        } else {
                            error = false
                        }
                    }
                }

                //  Поле ввода пароля
                TextField {
                    id: passwordField
                    Layout.preferredWidth: 200
                    Layout.alignment: Qt.AlignHCenter
                    color: "black"
                    placeholderText: "Введите пароль"
                    echoMode: TextField.Password    //  Скрывает вводимые символы точками
                }

                RowLayout {
                    Layout.preferredWidth: implicitWidth
                    Layout.alignment: Qt.AlignHCenter

                    Button {
                        text: "Войти"
                        Layout.preferredWidth: 100
                        highlighted: true
                        onClicked: {
                            // Вызываем C++ функцию проверки
                            //root.authBackend.login(usernameField.text, passwordField.text)
                            AuthController.autentification(usernameField.text, passwordField.text)
                        }
                    }

                    Button {
                        text: "Регистрация"
                        Layout.preferredWidth: 100
                        highlighted: true
                        onClicked: {
                            mainStack.push(registration)
                            root.title = "Registration"
                        }
                    }
                }

                // Текст для вывода ошибок
                Text {
                    id: errorText
                    text: root.errorMessage
                    color: "#EF4444" // Красный цвет ошибки
                    Layout.fillWidth: true
                    wrapMode: Text.Wrap
                    horizontalAlignment: Text.AlignHCenter
                }
            }
        }
    }

    Component {
        id: registration

        Page {
            background: Rectangle { color: "red"; radius: 16 }
            anchors.margins: 20
            Column {
                anchors.centerIn: parent
                anchors.margins: 20
                spacing: 20
                Text {
                    text: "New User registration page"
                }
                Button {
                    text: "Back"
                    onClicked: {
                        mainStack.pop()
                        root.title = "LogIn"
                    }
                }
            }
        }
    }
}
