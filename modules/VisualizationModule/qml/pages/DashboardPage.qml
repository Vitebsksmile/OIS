import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


Page {
    id: dashboardPage
    title: "Dashboard"

    // Переопределяем фон страницы
    background: Rectangle {
        color: "transparent"//"#f0f0f0"
        radius: 16
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 5

        // ===== Центральная часть. График =====
        InspectionStatisticsModel {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        // ===== Верхняя панель статистики =====
        InspectionSummaryModel {
            Layout.fillWidth: true
            Layout.preferredHeight: implicitHeight
        }
    }

}
