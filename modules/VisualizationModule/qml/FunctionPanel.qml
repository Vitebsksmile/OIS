import QtQuick
import QtQuick.Layouts


Rectangle {

    id: root

    implicitWidth: internalLayout.implicitWidth + internalLayout.anchors.margins * 2

    //  Связи (мостики) к внешним ресурсам
    property var targetHandler: null //  Текущий экземпляр FileHandler
    property var targetPopup: null //  Всплывающее окно

    color: Qt.rgba(44 / 255, 62 / 255, 80 / 255, 0.9) //  Midnight Blue
    radius: 10

    RowLayout {

        id: internalLayout

        anchors.fill: parent // Растягиваем на весь родительский элемент

        anchors.margins: 10 //  Внутренние отступы от краев родителя
        spacing: 5 //  Расстояние между кнопками

        FunctionPanelButton {

            Layout.preferredHeight: implicitHeight //  Растягиваем кнопку на всю высоту родителя
            Layout.preferredWidth: implicitWidth // Задаем идеальную высоту кнопки на основе содержимого

            text: qsTr("Open") //  Текст с поддержкой перевода

            onClicked: {

                imagePicker.mode = "open" //  Переключаем диалог в режим открытия
                imagePicker.open() //  Показываем окно выбора
            }
        }

        FunctionPanelButton {

            Layout.preferredHeight: implicitHeight //  Растягиваем кнопку на всю высоту родителя
            Layout.preferredWidth: implicitWidth // Задаем идеальную высоту кнопки на основе содержимого

            text: qsTr("Save as") //  Текст с поддержкой перевода

            onClicked: {

                imagePicker.mode = "save" //  Переключаем диалог в режим сохранения
                imagePicker.open() //  Показываем окно выбора
            }
        }

        FunctionPanelButton {

            id: start

            Layout.preferredHeight: implicitHeight //  Растягиваем кнопку на всю высоту родителя
            Layout.preferredWidth: implicitWidth // Задаем идеальную высоту кнопки на основе содержимого

            text: qsTr("Start preprocessing") //  Текст с поддержкой перевода

            enabled: root.targetHandler != ""

            //  Метод для старта предобработки изображения
            onClicked: root.startPreprocessing()
        }

        /*Item {
            Layout.fillWidth: true //  Заполняем всю оставшуюся высоту родителя
        }*/
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
