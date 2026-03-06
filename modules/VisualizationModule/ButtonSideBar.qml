import QtQuick 2.15
import QtCore
import QtQuick.Dialogs
import QtQuick.Layouts
import QtQuick.Controls 2.15
import QtQuick.Controls.Basic


ColumnLayout {

    //  связи (мостики) к внешним объектам
    property var targetHandler: null
    property var targetPopup: null

    anchors.fill: parent
    anchors.margins: 3
    spacing: 3

    Button {
        text: "Выбрать фото"
        Layout.fillWidth: true

        background: Rectangle {
            color: parent.down ? "#bbbbbb" : "#ffffff"
            border.color: "#333333"
            radius: 10
        }

        onClicked: openDialog.open()
    }

    Button {
        text: "Сохранить фото"
        Layout.fillWidth: true
        background: Rectangle {
            color: parent.down ? "#bbbbbb" : "#ffffff"
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
            if (targetHandler)
            {
                targetHandler.selectImage(selectedFile)
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
            if (targetHandler && targetPopup)
            {
                let success = targetHandler.saveImage(targetHandler.currentImagePath, selectedFile)
                targetPopup.message = success ? "Сохранено успешно!" : "Ошибка сохранения"
                targetPopup.open()
            }
        }
    }
}
