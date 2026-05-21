import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// ===== Верхняя панель статистики =====
RowLayout {
    id: layout
    implicitWidth: layout.implicitWidth + (layout.anchors.margins * 2)
    implicitHeight: layout.implicitHeight + (layout.anchors.margins * 2)
    spacing: 5

    //  1. Проверенные изображения
    Rectangle {
        Layout.fillWidth: true
        implicitHeight: rowFirst.implicitHeight + (rowFirst.anchors.margins * 2)
        Layout.preferredHeight: implicitHeight

        radius: 10
        color: "white"
        border.color: "#dcdcdc"

        Row {
            id: rowFirst
            anchors.centerIn: parent
            anchors.margins: 5
            spacing: 6

            Label {
                text: qsTr("Inspected Items")   //  Проверенные элементы
                font.pixelSize: 15
                color: "#555"
                anchors.verticalCenter: parent.verticalCenter
            }

            Label {
                text: "1248"
                font.pixelSize: 18
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

    //  2. Обнаруженные дефекты
    Rectangle {
        Layout.fillWidth: true
        implicitHeight: rowSecond.implicitHeight + (rowSecond.anchors.margins * 2)
        Layout.preferredHeight: implicitHeight

        radius: 10
        color: "white"
        border.color: "#dcdcdc"

        Row {
            id: rowSecond
            anchors.centerIn: parent
            anchors.margins: 5
            spacing: 6

            Label {
                text: qsTr("Detected Defects")
                font.pixelSize: 15
                color: "#555"
                anchors.verticalCenter: parent.verticalCenter
            }

            Label {
                text: "86"
                font.pixelSize: 18
                font.bold: true
                color: "#d32f2f"
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

    //  3. Точность (Accuracy)
    //  Показывает общую долю правильных ответов модели
    //  (как положительных, так и отрицательных) среди всех примеров.
    Rectangle {
        Layout.fillWidth: true
        implicitHeight: rowThird.implicitHeight + (rowThird.anchors.margins * 2)
        Layout.preferredHeight: implicitHeight

        radius: 10
        color: "white"
        border.color: "#dcdcdc"

        Row {
            id: rowThird
            anchors.centerIn: parent
            anchors.margins: 5
            spacing: 6

            Label {
                text: qsTr("Accuracy")      //  Точность
                font.pixelSize: 15
                color: "#555"
                anchors.verticalCenter: parent.verticalCenter
            }

            Label {
                text: "97.8 %"
                font.pixelSize: 18
                font.bold: true
                color: "#2e7d32"
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }
}
