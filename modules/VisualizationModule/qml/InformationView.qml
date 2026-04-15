import QtQuick
import QtQuick.Layouts


Rectangle {

    id: root

    //  Связи (мостики) к внешним ресурсам
    property var targetHandler: null //  Текущий экземпляр FileHandler

    width: 200
    implicitHeight: layout.implicitHeight + (layout.anchors.margins * 2)
    color: "green"
    radius: 10


    ColumnLayout {
        id: layout

        anchors.fill: parent
        anchors.margins: 10
        spacing: 5


        Text {
            id: fileText

            Layout.fillWidth: true
            wrapMode: Text.WrapAnywhere
            Layout.preferredHeight: contentHeight

            text: "Путь: " + root.targetHandler.getCleanPath(root.targetHandler.currentImagePath)

        }


        Text {

            Layout.fillWidth: true

            text: "Размер: " + root.targetHandler.getFileSize(root.targetHandler.currentImagePath).toFixed(2) + " MB"
        }

    }

}
