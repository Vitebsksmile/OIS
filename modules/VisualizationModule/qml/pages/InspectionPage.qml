import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


Page {
    id: inspectionPage
    title: "Inspection"


    //  Экспортируем внутренние объекты наружу,
    //  чтобы обращаться к ним как <id>.handler или <id>.popup
    property alias handler: sourceViewer.handler //  Текущий путь к изображению
    property alias popup: sourceViewer.popup //  Всплывающее уведомление
    property alias statusText: sourceViewer.statusText //      Задать текст статуса загрузки изображения снаружи

    // Переопределяем фон страницы
    background: Rectangle {
        color: "transparent"//"#f0f0f0"
        radius: 16
    }

    RowLayout {
        anchors.fill: parent

        ControlPanel {
            id: controlPanel

            Layout.preferredWidth: implicitWidth
            Layout.fillHeight: true

            border.color: "#e2e8f0"
            //color: "#f0f0f0"

            //  Прокидываем в ControlPanel.qml ссылки (alias) из ImageView.qml,
            //  используя экземпляр его родительского объекта с id: sourceViewer
            targetHandler: sourceViewer.handler
            targetPopup: sourceViewer.popup
        }

        ColumnLayout {

            Layout.fillWidth: true
            Layout.fillHeight: true

            Layout.minimumWidth: 200
            Layout.minimumHeight: 200

            spacing: 5

            ImageView {
                id: sourceViewer
                Layout.fillWidth: true
                Layout.fillHeight: true

                border.color: "#e2e8f0"
                color: "#f0f0f0"

                Layout.minimumWidth: 200
                Layout.minimumHeight: 200
            }

            InformationView {

                Layout.fillWidth: true
                Layout.preferredHeight: implicitHeight

                border.color: "#e2e8f0"
                color: "#f0f0f0"

                //  Прокидываем в InformationView.qml ссылки (alias) из ImageView.qml,
                //  используя экземпляр его родительского объекта с id: sourceViewer
                targetHandler: sourceViewer.handler
            }
        }
    }
}
