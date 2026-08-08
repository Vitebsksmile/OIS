import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


Rectangle {

    id: root

    //  Export properties,
    //  чтобы обращаться к ним как <id>.handler или <id>.popup
    property alias handler: sourceViewer.handler //  Текущий экземпляр FileHandler
    property alias popup: sourceViewer.popup //  Всплывающее уведомление
    property alias backgroundColor: sourceViewer.backgroundColor //      Задать цвет снаружи
    property alias labelText: sourceViewer.labelText //      Задать текст статуса загрузки изображения снаружи

    color: "#f0f0f0"

    ColumnLayout {

        anchors.fill: parent

        ImageView {

            id: sourceViewer
            Layout.fillWidth: true
            Layout.fillHeight: true

            Layout.minimumWidth: 200
            Layout.minimumHeight: 200

            handler.directionOut: true

        }

        InformationView {
            id: informationView

            Layout.fillWidth: true
            Layout.preferredHeight: implicitHeight

            //  Прокидываем в InformationView.qml ссылки (alias) из ImageView.qml,
            //  используя экземпляр его родительского объекта с id: sourceViewer
            targetHandler: sourceViewer.handler

        }

    }

}
