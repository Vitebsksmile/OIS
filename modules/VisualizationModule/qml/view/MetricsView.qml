import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


// ===== Панель метрик =====
RowLayout {
    id: layout
    implicitWidth: layout.implicitWidth + (layout.anchors.margins * 2)
    implicitHeight: layout.implicitHeight + (layout.anchors.margins * 2)
    spacing: 5

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

            //  Precision (Точность/Прецизионность):
            //  Показывает, какая доля объектов,
            //  названных моделью положительными, действительно является положительными.
            Label {
                text: qsTr("Precision")
                font.pixelSize: 15
                color: "#666"
                anchors.verticalCenter: parent.verticalCenter
            }

            Label {
                text: "0.972"
                font.pixelSize: 18
                font.bold: true
                color: "#1976d2"
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

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
                text: qsTr("Recall")
                font.pixelSize: 15
                color: "#666"
                anchors.verticalCenter: parent.verticalCenter
            }

            Label {
                text: "0.948"
                font.pixelSize: 18
                font.bold: true
                color: "#388e3c"
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

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
                text: qsTr("F1-Score")
                font.pixelSize: 15
                color: "#666"
                anchors.verticalCenter: parent.verticalCenter
            }

            Label {
                text: "0.960"
                font.pixelSize: 18
                font.bold: true
                color: "#d32f2f"
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }
}
