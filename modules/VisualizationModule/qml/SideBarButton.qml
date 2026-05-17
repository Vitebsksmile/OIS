import QtQuick                  //  Базовые элементы (Rectangle, Image, Text)
import QtQuick.Controls         //  Стандартные элементы (Window, Button, Popup, Menu, SplitView)
import QtQuick.Controls.Basic   //  style


Button {

    id: controlButtonRoot

    property var targetHandler: null

    text: qsTr("Default")

    hoverEnabled: true // Шаг 1: Разрешаем отслеживание мыши


    contentItem: Row {

        spacing: 10 //  расстояние между иконкой и названием кнопки


        Text {

            text: controlButtonRoot.text
            wrapMode: Text.WordWrap
            width: parent.width
            verticalAlignment: Text.AlignVCenter;
            horizontalAlignment: Text.AlignHCenter

        }

    }


    background: Rectangle {

        color: controlButtonRoot.down ? "#bbbbbb" :
                                        controlButtonRoot.hovered ? "#e0e0e0" : "#ffffff"

        border.color: "#333333"
        radius: 10

    }

}
