import QtCore           //  Нужен для работы со StandardPath (пути ОС)
import QtQuick          //  Базовые элементы (Rectangle, Image, Text)
import QtQuick.Layouts  //  Работа с ColumnLayout
import QtQuick.Dialogs

//  Системные окна (MessageDialog)
Rectangle {

    id: sideBarRoot

    //  Связи (мостики) к внешним ресурсам
    property var targetHandler: null //  Текущий экземпляр FileHandler
    property var targetPopup: null //  Всплывающее окно

    color: Qt.rgba(44 / 255, 62 / 255, 80 / 255, 0.9) //  Midnight Blue
    radius: 10


    ColumnLayout {

        anchors.fill: parent //  Растягиваем колонку на всю ширину родителя
        anchors.margins: 10 //  Внутренние отступы от краев родителя
        spacing: 5 //  Расстояние между кнопками

        SideBarButton {

            Layout.fillWidth: true //  Растягиваем кнопку на всю ширину родителя

            text: qsTr("Open image") //  Текст с поддержкой перевода

            onClicked: {

                //globalDialog.mode = "open" //  Переключаем диалог в режим открытия
                //globalDialog.open() //  Показываем окно выбора
                imagePicker.mode = "open" //  Переключаем диалог в режим открытия
                imagePicker.open() //  Показываем окно выбора
            }
        }

        SideBarButton {

            Layout.fillWidth: true //  Растягиваем кнопку на всю ширину родителя

            text: qsTr("Save image") //  Текст с поддержкой перевода

            onClicked: {

                imagePicker.mode = "save" //  Переключаем диалог в режим сохранения
                imagePicker.open() //  Показываем окно выбора
            }
        }

        SideBarButton {

            id: start

            Layout.fillWidth: true

            text: qsTr("Start preprocessing")

            enabled: sideBarRoot.targetHandler != ""

            //  Метод для старта предобработки изображения
            onClicked: sideBarRoot.startPreprocessing()
        }


        Item {
            Layout.fillHeight: true
        }
    }


    ImagePickerDialog {
        id: imagePicker

        targetHandler: sideBarRoot.targetHandler
        targetPopup: sideBarRoot.targetPopup

    }


    function startPreprocessing() {

        if (sideBarRoot.targetHandler) {

            sideBarRoot.targetHandler.startPreprocessing()

        }
    }
}
