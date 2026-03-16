import QtCore
import QtQuick          //  Базовые элементы (Rectangle, Image, Text)
import QtQuick.Layouts  //  Продвинутая расстановка элементов (ColumnLayout, RowLayout)
import QtQuick.Dialogs          //  Системные окна (MessageDialog)

Rectangle {

    id: sideBarRoot

    //  связи (мостики) к внешним объектам
    property var targetHandler: null
    property var targetPopup: null

    color: Qt.rgba(44/255, 62/255, 80/255, 0.9) //  Midnight Blue
    radius: 10


    ColumnLayout {

        anchors.fill: parent
        anchors.margins: 10
        spacing: 5


        SideBarButton {

            Layout.fillWidth: true

            text: "Open image"

            onClicked: {
                globalDialog.mode = "open"
                globalDialog.open()
            }

        }


        SideBarButton {

            Layout.fillWidth: true

            text: "Save image"

            onClicked: {
                globalDialog.mode = "save"
                globalDialog.open()
            }

        }


        SideBarButton {

            Layout.fillWidth: true

            text: "Start pre-processing"

        }


        Item { Layout.fillHeight: true }

    }


    FileDialog {

        id: globalDialog

        property string mode: "open"

        title: mode === "save" ? "Save image" : "Select image"
        fileMode: mode === "save" ? FileDialog.SaveFile : FileDialog.OpenFile

        currentFolder: StandardPaths.writableLocation(StandardPaths.PicturesLocation)

        nameFilters: ["Изображения (*.png *.jpg *.jpeg)"]

        onAccepted: {

            console.log("Выбран режим:", mode)
            console.log("Путь к файлу:", selectedFile, "\n")


            if (mode === "open") {

                //  вызываем метод у переданного Handler
                if (sideBarRoot.targetHandler)
                {
                    sideBarRoot.targetHandler.selectImage(selectedFile)
                }

            } else {

                //  Здесь была бы логика сохранения файла через C++*/
                if (sideBarRoot.targetHandler && sideBarRoot.targetPopup)
                {
                    let success = sideBarRoot.targetHandler.saveImage(sideBarRoot.targetHandler.currentImagePath, selectedFile)
                    sideBarRoot.targetPopup.message = success ? "Saved successfully!" : "Save error!"
                    sideBarRoot.targetPopup.open()
                }

            }
        }

    }

}
