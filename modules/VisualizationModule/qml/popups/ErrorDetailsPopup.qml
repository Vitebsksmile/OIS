import QtQuick
import QtQuick.Controls

Popup {
    id: root

    property string errorTitle: ""
    property string errorDetails: ""

    closePolicy: Popup.CloseOnEscape | Popup.NoAutoClose

    parent: Overlay.overlay
    padding: 10
    x: Math.round((parent.width - width) / 2)
    y: Math.round((parent.height - height) / 2)

    modal: false //  Не блокирует взаимодействие с основным окном
    focus: false //  Не перехватывает ввод (Escape не сработает при false)

    visible: root.errorDetails !== ""

    // При закрытии окна автоматически очищаем текст ошибки в родителе
    onClosed: {
        root.errorTitle = ""
        root.errorDetails = ""
    }

    contentItem: Column {
        spacing: 10
        anchors.centerIn: parent

        TitleText {
            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Text.AlignHCenter
            text: root.errorTitle
        }

        NormalText {
            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Text.AlignHCenter
            text: root.errorDetails
        }

        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Close")
            highlighted: true
            onClicked: {
                root.close()
            }
        }
    }
}
