import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


ToolBar {

    id: floatingToolBar

    signal menuTriggered()

    // Переопределяем фон ToolBar
    background: Rectangle {
        color: "#c2c0c0"
        radius: 10
        border.color: "#e2e8f0"
    }

    // Привязываем ToolBar с учетом левого и правого маргина
    anchors.left: parent.left
    anchors.leftMargin: parent.anchors.leftMargin
    anchors.right: parent.right
    anchors.rightMargin: parent.anchors.rightMargin
    anchors.top: parent.top

    implicitWidth: headerLayout.implicitWidth + (headerLayout.anchors.margins * 2)
    implicitHeight: headerLayout.implicitHeight + (headerLayout.anchors.margins * 2)

    RowLayout {

        id: headerLayout

        anchors.fill: parent
        anchors.margins: 3

        ToolButton {
            // Переопределяем фон страницы
            background: Rectangle {
                color: "transparent"
            }
            text: "☰"
            font.pixelSize: 28
            font.bold: true
            onClicked: floatingToolBar.menuTriggered()
        }

        Label {
            //Layout.alignment: Qt.AlignHCenter
            text: "Optical Inspection System"
            font.pixelSize: 18
            font.bold: true
        }

    }

}
