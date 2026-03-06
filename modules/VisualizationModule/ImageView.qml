import QtQuick 2.15
import QtQuick.Controls 2.15
import path

Rectangle {
    id: root
    implicitWidth: 200
    implicitHeight: 200
    color: "lightblue"
    radius: 10

    property alias handler: fileHandler
    property alias popup: statusPopup
    property alias imageSource: mainImage.source

    FileHandler {id: fileHandler }

    Image {
        id: mainImage
        anchors.fill: parent
        anchors.margins: 5
        fillMode: Image.PreserveAspectFit

        //  автоматическая связь с c++ свойством
        source: fileHandler.currentImagePath

        //  плавное появление фото
        Behavior on opacity { NumberAnimation { duration: 500 } }
    }

    Text {
        visible: mainImage.status === Image.Null
        anchors.centerIn: parent
        text: "фото не выбрано"
        color: "gray"
    }

    //  всплывающее уведомление (Toast)
    Popup {
        id: statusPopup
        property string message: ""
        x: (parent.width - width) / 2
        y: (parent.height - 100)
        width: 200
        height: 40
        Text {
            anchors.centerIn: parent;
            text: statusPopup.message
        }
    }
}
