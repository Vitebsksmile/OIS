import QtQuick
import QtQuick.Layouts
import QtQuick.Controls


pragma ComponentBehavior: Bound

Item {
    id: root
    anchors.fill: parent

    // 1. Модель данных, где хранятся наши вкладки
    ListModel {
        id: tabsModel
        Component.onCompleted: {
            // Создаем одну вкладку по умолчанию при старте
            tabsModel.append({ "title": "Image Processing", "viewSource": "HomeView.qml" })
        }
    }

    // Главная подложка (Цвет панели вкладок Chrome)
    Rectangle {
        anchors.fill: parent
        color: "#e0e0e0"
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Панель вкладок + кнопка "+"
        RowLayout {
            Layout.fillWidth: true
            Layout.topMargin: 4 // Небольшой отступ сверху экрана
            spacing: 5

            TabBar {

                id: tabBar
                Layout.fillWidth: true
                currentIndex: 0


                // Убираем дефолтный фон самого TabBar, делая его прозрачным
                background: Rectangle { color: "transparent" }


                // Динамически создаем заголовки на основе модели
                Repeater {

                    model: tabsModel


                    delegate: BrowserTabButton {

                        // Логика закрытия вкладки
                        onCloseRequested: {

                            tabsModel.remove(index)

                            // Корректируем текущий индекс после удаления
                            if (tabBar.currentIndex >= tabsModel.count) {
                                tabBar.currentIndex = tabsModel.count - 1
                            }

                        }

                    }

                }

            }

            // Кнопка "+" для добавления новой вкладки
            Button {
                id: addButton

                text: "+"
                implicitWidth: 28
                implicitHeight: 28

                background: Rectangle {
                    radius: 14
                    color: addButton.hovered ? Qt.rgba(240,240,240, 0.15) : "transparent"
                }

                contentItem: Text {
                    text: addButton.text
                    color: "#474a4d"
                    font.pixelSize: 18
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                /*onClicked: {
                    var newIndex = tabsModel.count + 1
                    tabsModel.append({
                        "title": "Вкладка " + newIndex,
                        "viewSource": "HomeView.qml" // Любой ваш View
                    })

                    // Сразу переключаемся на созданную вкладку
                    tabBar.currentIndex = tabsModel.count - 1

                }*/

                // При клике открываем всплывающее меню выбора режима
                onClicked: modeMenu.open()

                // Всплывающее меню (Popup)
                Menu {

                    id: modeMenu
                    y: addButton.height + 4 //  Смещение чуть ниже кнопки

                    //  Настройка стиля менб по Windows 11
                    background: Rectangle {

                        implicitWidth: 180
                        color: "#FFFFFF"
                        border.color: "#E5E5E5"
                        radius: 6

                    }

                    MenuItem {
                        text: "📷 Открыть Камеру"
                        onTriggered: {
                            var newIndex = tabsModel.count + 1
                            tabsModel.append({
                                "title": "Камера " + newIndex,
                                "viewSource": "CameraProcessingView.qml"
                            })
                            tabBar.currentIndex = tabsModel.count - 1
                        }
                    }

                    MenuItem {
                        text: "🖼️ Обработка фото"
                        onTriggered: {
                            var newIndex = tabsModel.count + 1
                            tabsModel.append({
                                "title": "Фото " + newIndex,
                                "viewSource": "ImageProcessingView.qml"
                            })
                            tabBar.currentIndex = tabsModel.count - 1
                        }
                    }

                }

            }

        }

        // 2. Рабочая область (Сливается с активной вкладкой)
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#f0f0f0" // Точный цвет активной вкладки


            // Используем StackLayout, который автоматически показывает элемент,
            // индекс которого совпадает с currentIndex у TabBar
            StackLayout {

                anchors.fill: parent
                currentIndex: tabBar.currentIndex

                // Динамически загружаем контент для каждой вкладки
                Repeater {

                    model: tabsModel


                    delegate: Item {

                        required property int index
                        required property string viewSource

                        Loader {

                            anchors.fill: parent

                            // Используем свойство родительского Item
                            source: parent.viewSource

                        }

                    }

                }

            }

        }

    }

}
