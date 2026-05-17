import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


//  Правая часть окна (центральный контент)
Page {

    id: root

    implicitHeight: 400

    //  Экспортируем внутренние объекты наружу,
    //  чтобы обращаться к ним как <id>.handler или <id>.popup
    property alias handler: sourceViewer.handler //  Текущий путь к изображению
    property alias popup: sourceViewer.popup //  Всплывающее уведомление

    RowLayout {

        anchors.fill: parent
        anchors.margins: 5

        ImageView {
            id: sourceViewer
            Layout.fillWidth: true
            Layout.fillHeight: true

            Layout.minimumWidth: 200
            Layout.minimumHeight: 400
        }

        ColumnLayout {

            Layout.preferredHeight: implicitHeight
            Layout.preferredWidth: implicitWidth

            Layout.minimumWidth: 100
            Layout.minimumHeight: 100

            InformationView {
                Layout.preferredHeight: implicitHeight

                //  Прокидываем в InformationView.qml ссылки (alias) из ImageView.qml,
                //  используя экземпляр его родительского объекта с id: sourceViewer
                targetHandler: sourceViewer.handler
            }

            ControlPanel {
                // Автоматический расчет ширины по содержимому + отступы по бокам
                //Layout.preferredWidth: implicitWidth
                Layout.preferredHeight: implicitHeight

                //  Прокидываем в ControlPanel.qml ссылки (alias) из ImageView.qml,
                //  используя экземпляр его родительского объекта с id: sourceViewer
                targetHandler: sourceViewer.handler
                targetPopup: sourceViewer.popup
            }

            Item {
                Layout.fillHeight: true //  Заполняем всю оставшуюся высоту родителя
            }
        }

    }

}
