import QtQuick                  //  Базовые элементы (Rectangle, Image, Text)
import QtQuick.Controls         //  Стандартные элементы (Window, Button, Popup, Menu, SplitView)
import QtQuick.Controls.Basic   //  style


Button {

    id: controlButtonRoot

    property var targetHandler: null

    text: qsTr("Default")


    // Идеальная ширина = ширина контента + внутренние отступы (padding) слева и справа
    implicitWidth: internalLayout.implicitWidth + leftPadding + rightPadding
    // Идеальная высота = высота контента + внутренние отступы сверху и снизу
    implicitHeight: internalLayout.implicitHeight + topPadding + bottomPadding


    // Задаем внутренние отступы кнопки (свободное пространство вокруг текста/иконки)
    padding: 10
    leftPadding: 15
    rightPadding: 15


    background: Rectangle {

        color: controlButtonRoot.down ? "#bbbbbb" : "#ffffff"
        border.color: "#333333"
        radius: 10

    }


    contentItem: Row {

        id: internalLayout
        spacing: 10 //  расстояние между иконкой и названием кнопки

        // Выравниваем Row строго по центру доступного пространства кнопки
        anchors.centerIn: parent


        Text {

            text: controlButtonRoot.text
            wrapMode: Text.WordWrap
            //width: parent.width
            verticalAlignment: Text.AlignVCenter;
            horizontalAlignment: Text.AlignHCenter

        }
    }

}
