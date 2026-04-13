import QtQuick
import QtQuick.Controls

Item {

    anchors.fill: parent

    //  Основной контейнер с разделителем (тянется мышкой)
    SplitView {

        id: mainSplitView

        orientation: Qt.Horizontal  //  Панели стоят в ряд (слева направо)
        anchors.fill: parent    //  Заполняем контейнером всю площадь родителя
        anchors.margins: 5


        //  Левая панель - боковое меню
        SideBar {

            id: sideBar

            //  Настройки размеров для mainSplitView
            SplitView.preferredWidth: 200   //  Желаемая ширина при старте
            SplitView.minimumWidth: 100     //  Минимальный порог сжатия
            SplitView.maximumWidth: 300     //  Максимальный порог растяжения

            //  Прокидываем в SideBar.qml ссылки (alias) из ImageView.qml,
            //  используя экземпляр его родительского объекта с id: centralContent
            targetHandler: centralContent.handler
            targetPopup: centralContent.popup

        }


        //  Правая часть окна (центральный контент)
        SplitView {

            id: root

            orientation: Qt.Vertical    //  Панели стоят колонной (сверху вниз)

            //  Настройки размеров для mainSplitView
            SplitView.fillWidth: true   //  Занимает всё оставшееся место справа


            //  Рабочая область
            ImageProcessingMode {
                id: centralContent

                //  Настройки размеров для root
                SplitView.fillHeight: true   //  Занимает всё оставшееся место справа

            }


            SplitView {

                id: otherContainer

                orientation: Qt.Horizontal  //  Панели стоят в ряд (слева направо)

                //  Настройки размеров для root
                SplitView.preferredHeight: 200   //  Желаемая высота при старте
                SplitView.minimumHeight: 100     //  Минимальный порог сжатия
                SplitView.maximumHeight: 500     //  Максимальный порог растяжения

                Rectangle {

                    color: Qt.rgba(255/255, 71/255, 77/255, 0.9) //  Light Red
                    radius: 10

                    //  Настройки размеров для otherContainer
                    SplitView.preferredWidth: 200   //  Желаемая ширина при старте
                    SplitView.minimumWidth: 100     //  Минимальный порог сжатия
                    SplitView.maximumWidth: 500     //  Максимальный порог растяжения

                }

                Rectangle {

                    color: Qt.rgba(255/255, 71/255, 77/255, 0.9) //  Light Red
                    radius: 10

                    //  Настройки размеров для otherContainer
                    SplitView.preferredWidth: 200   //  Желаемая ширина при старте
                    SplitView.minimumWidth: 100     //  Минимальный порог сжатия
                    SplitView.maximumWidth: 500     //  Максимальный порог растяжения

                }

                Rectangle {

                    color: Qt.rgba(255/255, 71/255, 77/255, 0.9) //  Light Red
                    radius: 10

                    //  Настройки размеров для otherContainer
                    SplitView.fillWidth: true       //  Занимает всё оставшееся место справа
                    SplitView.minimumWidth: 100     //  Минимальный порог сжатия

                }

            }

        }

    }
}
