import QtQuick
import QtQuick.Layouts


Rectangle {

    id: root

    //  Связи (мостики) к внешним ресурсам
    property var targetHandler: null //  Текущий экземпляр FileHandler

    implicitWidth: layout.implicitWidth + (layout.anchors.margins * 2)
    implicitHeight: layout.implicitHeight + (layout.anchors.margins * 2)
    color: "lightgreen"
    radius: 10


    ColumnLayout {
        id: layout
        anchors.fill: parent
        anchors.margins: 10
        spacing: 5


        Text {
            id: fileText
            Layout.fillWidth: true
            Layout.preferredHeight: contentHeight

            wrapMode: Text.WrapAnywhere
            text: qsTr("Sourse path: ") + root.targetHandler.getCleanPath(root.targetHandler.currentImagePath)

        }


        Text {

            Layout.fillWidth: true

            text: qsTr("Size: ") + root.targetHandler.getFileSize(root.targetHandler.currentImagePath).toFixed(2) + " MB"
        }

    }

}
