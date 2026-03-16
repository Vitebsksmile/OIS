import QtCore
import QtQuick
import QtQuick.Dialogs

FileDialog {

    id: imageDialog

    //  Связи (мостики) к внешним объектам
    property var secondTargetHandler: null
    //property var targetPopup: null


    title: "Выберите изображение"
    currentFolder: StandardPaths.writableLocation(StandardPaths.PicturesLocation)
    nameFilters: ["Изображения (*.png *.jpg *.jpeg)"]

    onAccepted: {

        //  Вызываем метод у переданного Handler
        if (imageDialog.secondTargetHandler)
        {
            imageDialog.secondTargetHandler.selectImage(selectedFile)
        }

    }

}
