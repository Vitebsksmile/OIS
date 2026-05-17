import QtQuick
import QtQuick.Controls


TabButton {
    id: control
    implicitWidth: 160
    implicitHeight: 36

    required property int index
    required property string title

    signal closeRequested() // Сигнал, который сообщает о нажатии на крестик

    contentItem: Item {
        anchors.fill: parent

        Text {
            text: control.title
            anchors.left: parent.left
            anchors.right: closeButton.left
            anchors.leftMargin: 12
            anchors.rightMargin: 4
            anchors.verticalCenter: parent.verticalCenter

            //  Активный текст ярче
            color: control.checked ? "#FFFFFF" : "#9AA0A6"
            font.pixelSize: 12
            elide: Text.ElideRight // Обрезает длинный текст как в браузере (слово...)
        }

        // Кнопка закрытия вкладки (крестик)
        Button {
            id: closeButton
            width: 20; height: 20
            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            text: "×"
            flat: true

            //  стилизуем крестик закрытия
            background: Rectangle {
                radius: 8
                color: closeButton.hovered ? Qt.rgba(1,1,1, 0.15) : "transparent"
            }

            contentItem: Text {
                text: closeButton.text
                color: control.checked ? "#FFFFFF" : "#9AA0A6"
                font.pixelSize: 14
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            // Чтобы клик по крестику не переключал вкладку, перехватываем событие
            onClicked: {
                control.closeRequested()
            }
        }

    }

    // ЭФФЕКТ СЛИЯНИЯ: Кастомный фон вкладки
    background: Rectangle {
        id: bgRect
        // Если вкладка активна (checked), красим в цвет контента (#f0f0f0),
        // если неактивна — в темный цвет панели вкладок (#e0e0e0)
        // при наведении красим в (#c9c9c9)
        color: control.checked ? "#f0f0f0" : (control.hovered ? "#c9c9c9" : "#e0e0e0")

        // Скругляем только верхние углы как в Chrome
        topLeftRadius: 8
        topRightRadius: 8
        bottomLeftRadius: 0
        bottomRightRadius: 0

        // Плавный переход цвета при наведении
        Behavior on color { ColorAnimation { duration: 100 } }
    }

}
