import QtCore
import QtQuick
import QtQuick.Dialogs
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Basic


ColumnLayout {

    id: root

    //  связи (мостики) к внешним объектам
    property var targetHandler: null
    property var targetPopup: null

    anchors.fill: parent
    anchors.margins: 3
    spacing: 3

    Button {
        id: openButton
        text: "Выбрать фото"
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
        text: "Сохранить фото"
        Layout.fillWidth: true
        background: Rectangle {
            color: (parent as Button).down ? "#bbbbbb" : "#ffffff"
            border.color: "#333333"
            radius: 10
        }

        onClicked: saveDialog.open()
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
            if (root.targetHandler)
            {
                root.targetHandler.selectImage(selectedFile)
            }

            //mainImage.source = selectedFile
            //console.log("Выбран файл: ", fileHandler.getCleanPath(selectedFile))
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
            if (root.targetHandler && root.targetPopup)
            {
                let success = root.targetHandler.saveImage(root.targetHandler.currentImagePath, selectedFile)
                root.targetPopup.message = success ? "Сохранено успешно!" : "Ошибка сохранения"
                root.targetPopup.open()
            }
        }
    }
}
