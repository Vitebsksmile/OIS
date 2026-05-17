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

        orientation: Qt.Vertical  //  Панели стоят в ряд (слева направо)
        anchors.fill: parent    //  Заполняем контейнером всю площадь родителя
        anchors.margins: 5

        DashboardPage {
            id: dashboardPage

            //  Настройки размеров для root
            SplitView.preferredHeight: dashboardPage.implicitHeight   //  Желаемая высота при старте
            SplitView.minimumHeight: 100     //  Минимальный порог сжатия
            SplitView.maximumHeight: 500     //  Максимальный порог растяжения
        }

        InspectionPage {
            id: inspectionPage

            //  Настройки размеров для root
            SplitView.fillHeight: true
            //SplitView.preferredHeight: inspectionPage.implicitHeight   //  Желаемая высота при старте
            SplitView.minimumHeight: 300     //  Минимальный порог сжатия
            SplitView.maximumHeight: 700     //  Максимальный порог растяжения

            //SplitView.minimumWidth: 600

        }

        StatisticsPage {
            id: statisticsPage

            //  Настройки размеров для root
            //SplitView.preferredHeight: inspectionPage.implicitHeight   //  Желаемая высота при старте
            SplitView.preferredHeight: 230
            SplitView.minimumHeight: 230     //  Минимальный порог сжатия
            SplitView.maximumHeight: 500     //  Максимальный порог растяжения

        }
    }
}
