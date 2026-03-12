//import QtCore
import QtQuick          //  Базовые элементы (Rectangle, Image, Text)
import QtQuick.Controls //  Стандартные элементы (Window, Button, Popup, Menu, SplitView)
import QtQuick.Layouts  //  Продвинутая расстановка элементов (ColumnLayout, RowLayout)
import QtQuick.Dialogs  //  Системные окна (MessageDialog)


ApplicationWindow {
    id: window
    width: 1200; height: 860
    visible: true


    //  Считываем имя приложения из настроек проекта (устанавливается в main.cpp)
    readonly property string appName: Qt.application.name


    //  1. Описание логики действий (Actions) - это "мозг" кнопок и меню
    Action {
        id: openAction
        text: "&Открыть..."         //  Символ '&' позволяет нажать Alt+О для активации
        shortcut: StandardKey.Open  //  Автоматически подхватывает Ctrl+O (Win) или Cmd+O (Mac)
        onTriggered: console.log(`Нажали ${openAction.text}`)
    }

    Action {
        id: exitAction
        text: "&Выход"
        shortcut: "Ctrl+Q"  //  Быстрое сочетание клавиш
        onTriggered: console.log(`Нажали ${exitAction.text}`) | Qt.quit()  //  Стандартная ф-ция выхода
    }


    // 2. Добавляем полосу меню
    menuBar: MenuBar {

        Menu {

            id: fileMenu
            title: "&Файл"

            // Привязываем пункты меню к ранее созданным Action
            MenuItem { action: openAction }
            MenuItem { action: exitAction }

        }

        Menu {

            id: editMenu
            title: "&Правка"

            MenuItem { text: "Копировать"; enabled: false }    //  Пример неактивного пункта
            MenuItem { text: "Вставить" }

        }

        Menu {

            id: helpMenu
            title: "&Помощь"

            MenuItem {
                text: "О программе"
                onTriggered: aboutDialog.open()
            }

        }
    }


    //  Системное окно с информацией о программе
    MessageDialog {
        id: aboutDialog
        title: "О программе"
        text: window.appName + "\nВерсия 1.0.0\nРазработано на Qt 6"
        buttons: MessageDialog.Ok
    }


    //  Основной контейнер с разделителем (тянется мышкой)
    SplitView {

        id: mainSplitView
        orientation: Qt.Horizontal  //  Панели стоят в ряд (слева направо)

        anchors.fill: parent    //  Заполняем контейнером всю площадь родителя
        anchors.margins: 5

        //  Левая панель - боковое меню
        Rectangle {

            id: sideBar
            color: Qt.rgba(44/255, 62/255, 80/255, 0.9) //  Midnight Blue
            radius: 10

            //  Настройки размеров для mainSplitView
            SplitView.preferredWidth: 200   //  Желаемая ширина при старте
            SplitView.minimumWidth: 100     //  Минимальный порог сжатия
            SplitView.maximumWidth: 300     //  Максимальный порог растяжения

            // Пользовательский компонент с кнопками
            ButtonSideBar {
                //  Прокидываем в ButtonSideBar.qml ссылки (alias) из ImageView.qml,
                //  используя экземпляр его объекта с id: sourceImageViewer
                targetHandler: sourceImageViewer.handler
                targetPopup: sourceImageViewer.popup
            }

        }


        //  Правая часть окна (центральный контент)
        SplitView {

            id: centralSplitContent
            orientation: Qt.Vertical    //  Панели стоят колонной (слева направо)

            //  Настройки размеров для mainSplitView
            SplitView.fillWidth: true   //  Занимает всё оставшееся место справа

            //  Вложенный разделитель для двух окон просмотра изображений
            //  (исходного изображения и предобработанного)
            SplitView {

                id: viewSplitContainer
                orientation: Qt.Horizontal  //  Панели стоят в ряд (слева направо)

                //  Настройки размеров для centralSplitContent
                SplitView.fillHeight: true  //  Занимает всё оставшееся место сверху

                //  Левое окно: Исходное изображение
                //  (экземпляр компонента, определенного в ImageView.qml)
                ImageView {

                    id: sourceImageViewer

                    //  Настройки размеров для viewSplitContainer
                    SplitView.preferredWidth: viewSplitContainer.width / 2  //  половина ширины
                    //  Минимальный порог сжатия
                    SplitView.minimumWidth: 100

                }

                //  Правое окно: Результат обработки (нормализация и т.д.)
                PreProcessedImageView {

                    id: processedImageView

                    //  Настройки размеров для viewSplitContainer
                    SplitView.preferredWidth: viewSplitContainer.width / 2  //  половина ширины
                    //  Минимальный порог сжатия
                    SplitView.minimumWidth: 100

                }

            }

            SplitView {

                id: buttomSplitContainer
                orientation: Qt.Horizontal  //  Панели стоят в ряд (слева направо)

                //  Настройки размеров для SplitView
                SplitView.preferredHeight: 200   //  Желаемая высота при старте
                SplitView.minimumHeight: 100     //  Минимальный порог сжатия
                SplitView.maximumHeight: 500     //  Максимальный порог растяжения

                Rectangle {

                    color: Qt.rgba(255/255, 71/255, 77/255, 0.9) //  Light Red
                    radius: 10

                    //  Настройки размеров для buttomSplitContainer
                    SplitView.preferredWidth: 200   //  Желаемая ширина при старте
                    SplitView.minimumWidth: 100     //  Минимальный порог сжатия
                    SplitView.maximumWidth: 500     //  Максимальный порог растяжения

                }

                Rectangle {

                    color: Qt.rgba(255/255, 71/255, 77/255, 0.9) //  Light Red
                    radius: 10

                    //  Настройки размеров для buttomSplitContainer
                    SplitView.preferredWidth: 200   //  Желаемая ширина при старте
                    SplitView.minimumWidth: 100     //  Минимальный порог сжатия
                    SplitView.maximumWidth: 500     //  Максимальный порог растяжения

                }

                Rectangle {

                    color: Qt.rgba(255/255, 71/255, 77/255, 0.9) //  Light Red
                    radius: 10

                    //  Настройки размеров для buttomSplitContainer
                    SplitView.fillWidth: true       //  Занимает всё оставшееся место справа
                    SplitView.minimumWidth: 100     //  Минимальный порог сжатия

                }

            }

        }

    }


    /*ListView {
        anchors.fill: parent
        model: myGlobalModel    //  ссылаемся на модель по имени, к-рое дидим в main.cpp
        spacing: 5

        //  DELEGATE: Как выглядит одна строка
        delegate: Rectangle {
            width: parent.width
            height: 40
            color: "lightgray"
            border.color: "darkgray"

            Text {
                anchors.centerIn: parent
                text: name  //  используем роль 'name' из нашей C++ модели
                color: "blue"
                font.pixelSize: 16
            }
        }
    }*/
}
