import QtQuick
import QtQuick.Controls

Popup {
    id: root

    property string errorTitle: ""
    property string errorDetails: ""
    property string message: "" //  Свое свойство для текста ошибки или успеха

    //  Центрируем по горизонтали и поднимаем на 100 пикселей от низа
    x: (parent.width - width) / 2
    y: parent.height - 100
    //width: Math.min(400, parent.width * 0.8)

    //  Настройки закрытия всплывающего окна
    modal: false //  Не блокирует взаимодействие с основным окном
    focus: false //  Не перехватывает ввод (Escape не сработает при false)

    visible: root.errorTitle !== ""

    // При закрытии окна автоматически очищаем текст ошибки в родителе
    onClosed: {
        root.errorTitle = ""
        root.errorDetails = ""
    }

    ErrorDetailsPopup {
        id: errorDetails
        errorDetails: root.errorDetails
    }

    //  Закроется, если нажать Esc (нужен focus: true) или кликнуть мимо
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    contentItem: Column {
        anchors.centerIn: parent
        spacing: 10
        TitleText {
            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Text.AlignHCenter
            //font.bold: Theme.fBoldTitle
            text: root.errorTitle
        }
        NormalText {
            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Text.AlignRight
            color: "blue"
            font.underline: true
            text: qsTr("more details about the error")

            MouseArea {
                width: parent.width
                height: parent.height
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    errorDetails.errorTitle = root.errorTitle
                    errorDetails.errorDetails = root.errorDetails
                    errorDetails.open()
                }
            }
        }
    }
}
