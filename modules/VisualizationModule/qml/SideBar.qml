//import QtCore           //  Нужен для работы со StandardPath (пути ОС)
import QtQuick          //  Базовые элементы (Rectangle, Image, Text)
import QtQuick.Layouts  //  Работа с ColumnLayout


//  Системные окна (MessageDialog)
Rectangle {

    id: root

    implicitHeight: internalLayout.implicitHeight + internalLayout.anchors.margins * 2

    //  Связи (мостики) к внешним ресурсам
    property var targetHandler: null //  Текущий экземпляр FileHandler
    property var targetPopup: null //  Всплывающее окно

    color: Qt.rgba(44 / 255, 62 / 255, 80 / 255, 0.0) //  Transparent
    radius: 10


    ColumnLayout {
        id: internalLayout

        //  Растягиваем по ширине
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        spacing: 10 //  Расстояние между кнопками

        ButtonBar {
            //  Настройки размеров для sideBar
            Layout.fillWidth: true
            Layout.preferredHeight: implicitHeight

            targetHandler: root.targetHandler
            targetPopup: root.targetPopup
        }

        CamcoderIpInput {
            //  Настройки размеров для sideBar
            Layout.fillWidth: true
            Layout.preferredHeight: implicitHeight
        }

        Item {
            Layout.fillHeight: true
        }
    }


    ImagePickerDialog {
        id: imagePicker

        targetHandler: root.targetHandler
        targetPopup: root.targetPopup

    }


    function startPreprocessing() {

        if (root.targetHandler) {

            root.targetHandler.startPreprocessing()

        }
    }
}
