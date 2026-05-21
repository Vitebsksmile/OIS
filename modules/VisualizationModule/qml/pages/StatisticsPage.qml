import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: root
    title: "Statistics"

    // Переопределяем фон страницы
    background: Rectangle {
        color: "transparent"//"#f0f0f0"
        radius: 16       // Радиус скругления углов
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 5

        // ===== Заголовок =====
        Label {
            text: qsTr("Inspection Statistics")
            font.pixelSize: 18
            font.bold: true
            color: "#222"
        }

        // ===== Центральная часть =====
        ColumnLayout {

            Layout.fillWidth: true
            Layout.fillHeight: true

            spacing: 5

            // ===== Верхняя панель метрик =====
            MetricsModel {
                Layout.fillWidth: true
                Layout.fillHeight: implicitHeight
            }

            // ===== График =====
            DetectionStatisticsModel {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            // ===== Таблица результатов =====
            StatisticsResultTableModel {
                Layout.fillWidth: true
                Layout.preferredHeight: 150
            }
        }
    }
}
