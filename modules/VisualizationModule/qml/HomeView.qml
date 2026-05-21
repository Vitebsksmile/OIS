import QtQuick
import QtQuick.Controls


Rectangle {

    anchors.fill: parent
    border.width: 20
    border.color: "#f0f0f0"
    anchors.bottomMargin: 10
    anchors.leftMargin: 5
    anchors.rightMargin: 5
    radius: 15

    //  Основной контейнер с разделителем (тянется мышкой)
    SplitView {
        id: root

        orientation: Qt.Horizontal  //  Панели стоят в ряд (слева направо)
        anchors.fill: parent    //  Заполняем контейнером всю площадь родителя
        anchors.margins: 5

        InspectionPage {
            id: inspectionPage

            //  Настройки размеров для root
            SplitView.fillWidth: true       //  на всю оставшуюся ширину
            SplitView.minimumWidth: 300     //  Минимальный порог сжатия
        }

        //  Statistics. Глубокая аналитика
        StatisticsPage {
            id: statisticsPage

            //  Настройки размеров для root
            SplitView.preferredWidth: 450   //  Желаемая высота при старте
            SplitView.minimumWidth: 420     //  Минимальный порог сжатия
            SplitView.maximumWidth: 500     //  Максимальный порог растяжения

        }
    }
}
