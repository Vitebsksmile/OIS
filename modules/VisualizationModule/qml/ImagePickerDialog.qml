import QtCore
import QtQuick
import QtQuick.Dialogs


FileDialog {

    id: globalDialog

    //  Связи (мостики) к внешним ресурсам
    property var targetHandler: null //  Текущий экземпляр FileHandler
    property var targetPopup: null //  Всплывающее окно

    //  Кастомное св-во для переключения между "Открыть" и "Сохранить"
    property string mode: "open"

    //  Динамический заголовок в зависимости от режима
    title: mode === "save" ? "Save image" : "Select image"

    //  В Qt6 режим (Open/Save) задается через перечисление fileMode
    fileMode: mode === "save" ? FileDialog.SaveFile : FileDialog.OpenFile

    //  Начинаем обзор в системной папке "Изображения"
    currentFolder: StandardPaths.writableLocation(
                       StandardPaths.PicturesLocation)

    //  Фильтр расширений файлов
    nameFilters: ["Изображения (*.png *.jpg *.jpeg)"]

    onAccepted: {

        console.log("Выбран режим:", mode)
        console.log("Путь к файлу:", selectedFile, "\n")

        //  Логика открытия
        if (mode === "open") {

            //  вызываем метод у переданного Handler
            if (globalDialog.targetHandler) {

                //  Передаем URL в C++, где сработает сигнал изменения пути
                globalDialog.targetHandler.selectImage(selectedFile)
            }
        }

        //  Логика сохранения
        if (mode === "save") {

            //  Логика сохранения файла через C++
            if (globalDialog.targetHandler && globalDialog.targetPopup) {

                //  Вызываем C++ метод копирования: из (текущий путь) в (путь из диалога)
                let success = globalDialog.targetHandler.saveImage(
                        globalDialog.targetHandler.currentImagePath,
                        selectedFile)

                //  Устанавливаем новый путь текущим путем
                //sideBarRoot.targetHandler.setCurrentImagePath(selectedFile)

                //  Настраиваем Toast-уведомление в зависимости от результата
                globalDialog.targetPopup.message
                        = success ? "Saved successfully!" : "Save error!"

                //  Показываем результат пользователю
                globalDialog.targetPopup.open()
            }
        }
    }
}
