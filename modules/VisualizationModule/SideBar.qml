import QtCore           //  Нужен для работы со StandardPath (пути ОС)
import QtQuick          //  Базовые элементы (Rectangle, Image, Text)
import QtQuick.Layouts  //  Работа с ColumnLayout
import QtQuick.Dialogs  //  Системные окна (MessageDialog)


Rectangle {

    id: sideBarRoot

    //  Связи (мостики) к внешним ресурсам
    property var targetHandler: null    //  Текущий путь к изображению
    property var targetPopup: null      //  Всплывающее окно

    color: Qt.rgba(44/255, 62/255, 80/255, 0.9)    //  Midnight Blue
    radius: 10


    ColumnLayout {

        anchors.fill: parent    //  Растягиваем колонку на всю ширину родителя
        anchors.margins: 10     //  Внутренние отступы от краев родителя
        spacing: 5              //  Расстояние между кнопками


        SideBarButton {

            Layout.fillWidth: true          //  Растягиваем кнопку на всю ширину родителя

            text: qsTr("Open image")        //  Текст с поддержкой перевода


            onClicked: {

                globalDialog.mode = "open"  //  Переключаем диалог в режим открытия
                globalDialog.open()         //  Показываем окно выбора

            }

        }


        SideBarButton {

            Layout.fillWidth: true          //  Растягиваем кнопку на всю ширину родителя

            text: qsTr("Save image")        //  Текст с поддержкой перевода


            onClicked: {

                globalDialog.mode = "save"  //  Переключаем диалог в режим открытия
                globalDialog.open()         //  Показываем окно выбора

            }

        }


        SideBarButton {

            Layout.fillWidth: true

            text: qsTr("Start pre-processing")

            enabled: sideBarRoot.targetHandler != ""

            //  Метод для копирования изображения в папку '/OIS/module/DatabaseModule/'
            onClicked: {

                //  Логика сохранения файла через C++
                if (sideBarRoot.targetHandler && sideBarRoot.targetPopup) {

                    //  Вызываем C++ метод копирования: из (текущий путь) в (путь из диалога)
                    //let success = sideBarRoot.targetHandler.saveImage(sideBarRoot.targetHandler.currentImagePath, selectedFile)

                    //  Настраиваем Toast-уведомление в зависимости от результата
                    //sideBarRoot.targetPopup.message = success ? "Saved successfully!" : "Save error!"

                    //  Показываем результат пользователю
                    sideBarRoot.targetPopup.open()

                }

            }

            //  Вызываем C++ метод копирования: из (текущий путь) в (путь системной папки гду будут храниться обработанные изображения)
            //let success = sideBarRoot.targetHandler.saveImage(sideBarRoot.targetHandler.currentImagePath, selectedFile)


        }


        Item { Layout.fillHeight: true }

    }


    FileDialog {

        id: globalDialog

        //  Кастомное св-во для переключения между "Открыть" и "Сохранить"
        property string mode: "open"

        //  Динамический заголовок в зависимости от режима
        title: mode === "save" ? "Save image" : "Select image"

        //  В Qt6 режим (Open/Save) задается через перечисление fileMode
        fileMode: mode === "save" ? FileDialog.SaveFile : FileDialog.OpenFile

        //  Начинаем обзор в системной папке "Изображения"
        currentFolder: StandardPaths.writableLocation(StandardPaths.PicturesLocation)

        //  Фильтр расширений файлов
        nameFilters: ["Изображения (*.png *.jpg *.jpeg)"]


        onAccepted: {

            console.log("Выбран режим:", mode)
            console.log("Путь к файлу:", selectedFile, "\n")

            //  Логика открытия
            if (mode === "open") {

                //  вызываем метод у переданного Handler
                if (sideBarRoot.targetHandler) {

                    //  Передаем URL в C++, где сработает сигнал изменения пути
                    sideBarRoot.targetHandler.selectImage(selectedFile)

                }

            }

            //  Логика сохранения
            if (mode === "save"){

                //  Логика сохранения файла через C++
                if (sideBarRoot.targetHandler && sideBarRoot.targetPopup) {

                    //  Вызываем C++ метод копирования: из (текущий путь) в (путь из диалога)
                    let success = sideBarRoot.targetHandler.saveImage(sideBarRoot.targetHandler.currentImagePath, selectedFile)

                    //  Настраиваем Toast-уведомление в зависимости от результата
                    sideBarRoot.targetPopup.message = success ? "Saved successfully!" : "Save error!"

                    //  Показываем результат пользователю
                    sideBarRoot.targetPopup.open()

                }

            }

        }

    }

}
