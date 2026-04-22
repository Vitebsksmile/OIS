import QtQuick
import QtQuick.Layouts


Rectangle {

    id: root

    implicitHeight: internalLayout.implicitHeight + internalLayout.anchors.margins * 2

    //  Связи (мостики) к внешним ресурсам
    property var targetHandler: null //  Текущий экземпляр FileHandler
    property var targetPopup: null //  Всплывающее окно

    color: Qt.rgba(44 / 255, 62 / 255, 80 / 255, 0.9) //  Midnight Blue
    radius: 10

    ColumnLayout {

        id: internalLayout

        //  Растягиваем по ширине
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top



        anchors.margins: 10 //  Внутренние отступы от краев родителя
        spacing: 5 //  Расстояние между кнопками

        SideBarButton {

            Layout.fillWidth: true //  Растягиваем кнопку на всю ширину родителя

            text: qsTr("Open") //  Текст с поддержкой перевода

            onClicked: {

                imagePicker.mode = "open" //  Переключаем диалог в режим открытия
                imagePicker.open() //  Показываем окно выбора
            }
        }

        SideBarButton {

            Layout.fillWidth: true //  Растягиваем кнопку на всю ширину родителя

            text: qsTr("Save as") //  Текст с поддержкой перевода

            onClicked: {

                imagePicker.mode = "save" //  Переключаем диалог в режим сохранения
                imagePicker.open() //  Показываем окно выбора
            }
        }

        SideBarButton {

            id: start

            Layout.fillWidth: true

            text: qsTr("Start preprocessing")

            enabled: root.targetHandler != ""

            //  Метод для старта предобработки изображения
            onClicked: root.startPreprocessing()
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
