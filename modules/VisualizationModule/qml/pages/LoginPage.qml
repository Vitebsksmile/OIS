import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: root
    title: "Login"

    property var authBackend
    property string errorMessage: ""
    //  Signals for navigation
    signal registrationClicked()
    signal helpClicked

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

        //  Поле ввода логина
        TextField {
            id: usernameField
            Layout.fillWidth: true
            placeholderText: "Введите логин"
            color: "black"
        }

        //  Поле ввода пароля
        TextField {
            id: passwordField
            Layout.fillWidth: true
            placeholderText: "Введите пароль"
            echoMode: TextField.Password    //  Скрывает вводимые символы точками
            color: "black"
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

        // Кнопка Войти
        Button {
            text: "Войти"
            Layout.fillWidth: true
            highlighted: true
            onClicked: {
                // Вызываем C++ функцию проверки
                root.authBackend.login(usernameField.text, passwordField.text)
            }
        }

        // Линия-разделитель
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 1
            color: "#475569"
        }

        // Нижние кнопки (Регистрация и Справка)
        Row {
            Layout.fillWidth: true
            spacing: 10

            Button {
                text: "Регистрация"
                width: (parent.width - 10) / 2
                onClicked: root.registrationClicked()
            }

            Button {
                text: "Справка"
                width: (parent.width - 10) / 2
                onClicked: root.helpClicked()
            }
        }

        // Отступ после разделителя
        Item { implicitHeight: 5 }
    }
}
