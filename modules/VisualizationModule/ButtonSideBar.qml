import QtCore
import QtQuick                  //  Базовые элементы (Rectangle, Image, Text)
import QtQuick.Dialogs          //  Системные окна (MessageDialog)
import QtQuick.Layouts          //  Продвинутая расстановка элементов (ColumnLayout, RowLayout)
import QtQuick.Controls         //  Стандартные элементы (Window, Button, Popup, Menu, SplitView)
import QtQuick.Controls.Basic   //  style


ColumnLayout {

    id: root

    //  связи (мостики) к внешним объектам
    property var oldTargetHandler: null
    property var targetPopup: null

    anchors.fill: parent
    anchors.margins: 3
    spacing: 3


    Button {
        id: oldOpenButton
        text: "Выбрать изображение"
        Layout.fillWidth: true

        background: Rectangle {
            color: (parent as Button).down ? "#bbbbbb" : "#ffffff"
            border.color: "#333333"
            radius: 10
        }

        onClicked: openDialog.open()
    }

    Button {
        id: saveButton
        text: "Сохранить изображение"
        Layout.fillWidth: true
        background: Rectangle {
            color: (parent as Button).down ? "#bbbbbb" : "#ffffff"
            border.color: "#333333"
            radius: 10
        }

        onClicked: saveDialog.open()
    }

    Button {
        id: preProcessingButton
        text: "Начать предварительную \nобработку"
        Layout.fillWidth: true
        background: Rectangle {
            color: (parent as Button).down ? "#bbbbbb" : "#ffffff"
            border.color: "#333333"
            radius: 10
        }
    }


    Item { Layout.fillHeight: true }

    //  диалоги
    FileDialog {
        id: openDialog
        title: "Выберите изображение"
        currentFolder: StandardPaths.writableLocation(StandardPaths.PicturesLocation)
        nameFilters: ["Изображения (*.png *.jpg *.jpeg)"]

        onAccepted: {
            //  вызываем метод у переданного Handler
            if (root.oldTargetHandler)
            {
                root.oldTargetHandler.selectImage(selectedFile)
            }
        }
    }

    FileDialog {
        id: saveDialog
        title: "Сохранить изображение"
        fileMode: FileDialog.SaveFile
        currentFolder: StandardPaths.writableLocation(StandardPaths.PicturesLocation)
        nameFilters: ["Изображения (*.png *.jpg *.jpeg)"]

        onAccepted: {
            //  Здесь была бы логика сохранения файла через C++*/
            if (root.oldTargetHandler && root.targetPopup)
            {
                let success = root.oldTargetHandler.saveImage(root.oldTargetHandler.currentImagePath, selectedFile)
                root.targetPopup.message = success ? "Сохранено успешно!" : "Ошибка сохранения"
                root.targetPopup.open()
            }
        }
    }

    FileDialog {
        id: preProcessingDialog
        title: ""
    }
}
