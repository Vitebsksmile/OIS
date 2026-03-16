import QtQuick          //  Базовые элементы (Rectangle, Image, Text)
import QtQuick.Controls //  Высокоуровневые компоненты (Popup, Button)


Rectangle {

    //id: root
    implicitWidth: 200
    implicitHeight: 200
    color: "lightgreen"
    radius: 10

    //  Экспортируем внутренние объекты наружу,
    //  чтобы обращаться к ним как root.handler или root.popup
    //property alias handler: fileHandler
    //property alias popup: statusPopup




}
