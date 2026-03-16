import QtQuick                  //  Базовые элементы (Rectangle, Image, Text)
import QtQuick.Controls         //  Стандартные элементы (Window, Button, Popup, Menu, SplitView)
import QtQuick.Controls.Basic   //  style


Button {

    id: controlButtonRoot

    property var targetHandler: null

    text: "Default"


    background: Rectangle {

        color: (parent as Button).down ? "#bbbbbb" : "#ffffff"
        border.color: "#333333"
        radius: 10

    }


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

}
