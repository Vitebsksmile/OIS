import QtQuick
import QtQuick.Layouts


Rectangle {

    id: root

    //  Связи (мостики) к внешним ресурсам
    property var targetHandler: null //  Текущий экземпляр FileHandler
    property var targetPopup: null //  Всплывающее окно

    // Внутренние отступы самого Rectangle от его краев до ColumnLayout
    property int leftPadding: 15
    property int rightPadding: 15
    property int topPadding: 15
    property int bottomPadding: 15

    implicitHeight: controlLayout.implicitHeight + (controlLayout.anchors.margins * 2)
    implicitWidth: 200

    color: Qt.rgba(44 / 255, 62 / 255, 80 / 255, 0.9) //  Midnight Blue
    radius: 10

    ColumnLayout {

        id: controlLayout

        //  Растягиваем по ширине
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.fill: parent


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

            Layout.fillWidth: true //  Растягиваем кнопку на всю ширину родителя

            text: qsTr("Start preprocessing") //  Текст с поддержкой перевода

            enabled: root.targetHandler != ""

            //  Метод для старта предобработки изображения
            onClicked: root.startPreprocessing()
        }

        Item {
            Layout.fillHeight: true //  Заполняем всю оставшуюся высоту родителя
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
