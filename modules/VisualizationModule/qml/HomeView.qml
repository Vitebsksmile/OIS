import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

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

            //  Настройки размеров для sideBar
            Layout.fillWidth: true
            Layout.preferredHeight: implicitHeight

            //  Прокидываем в SideBar.qml ссылки (alias) из ImageView.qml,
            //  используя экземпляр его родительского объекта с id: centralContent
            targetHandler: imageProcessingView.handler
            targetPopup: imageProcessingView.popup
        }

        //  Правая часть окна (центральный контент)
        SplitView {
            id: centralContainer

            orientation: Qt.Vertical    //  Панели стоят колонной (сверху вниз)

            //  Настройки размеров для mainSplitView
            SplitView.fillWidth: true   //  Занимает всё оставшееся место справа


            //  Рабочая область
            ImageProcessingView {
                id: imageProcessingView

                //  Настройки размеров для centralContainer
                SplitView.fillHeight: true   //  Занимает всё оставшееся место

            }

            //  Futter
            FutterView {
                id: futter

                //  Настройки размеров для centralContainer
                SplitView.preferredHeight: 200   //  Желаемая высота при старте
                SplitView.minimumHeight: 100     //  Минимальный порог сжатия
                SplitView.maximumHeight: 500     //  Максимальный порог растяжения
            }
        }
    }
}
