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
        anchors.margins: 10

        ImageView {
            id: sourceViewer
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        ControlPanel {
            implicitWidth: 200

            //  Прокидываем в ControlPanel.qml ссылки (alias) из ImageView.qml,
            //  используя экземпляр его родительского объекта с id: sourceViewer
            targetHandler: sourceViewer.handler
            targetPopup: sourceViewer.popup
        }

    }

}
