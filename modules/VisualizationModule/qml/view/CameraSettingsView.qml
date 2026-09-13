import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// =====================================================
// CAMERA SETTINGS
// =====================================================

Rectangle {
    width: ListView.view ? ListView.view.width : 0
    implicitHeight: rootLayout.implicitHeight + 32
    radius: 12

    color: "white"
    border.color: "#dcdcdc"

    ColumnLayout {
        id: rootLayout
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.topMargin: 16
        spacing: 14

        Label {
            Layout.preferredHeight: implicitHeight
            Layout.alignment: Qt.AlignVCenter
            text: "Camera Settings"
            font.pixelSize: 22
            font.bold: true
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: implicitHeight
            Layout.alignment: Qt.AlignVCenter

            Label {
                Layout.preferredWidth: 180
                Layout.alignment: Qt.AlignVCenter
                text: "Camera Device:"
            }

            ComboBox {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignVCenter

                model: [
                    "Default Camera",
                    "USB Camera #1",
                    "IP-camera",
                    "Industrial Camera"
                ]
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: implicitHeight
            Layout.alignment: Qt.AlignVCenter

            Label {
                Layout.preferredWidth: 180
                Layout.alignment: Qt.AlignVCenter
                text: "Resolution:"
            }

            ComboBox {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignVCenter

                model: [
                    "640x480",
                    "1280x720",
                    "1920x1080"
                ]
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: implicitHeight
            Layout.alignment: Qt.AlignVCenter

            Label {
                Layout.preferredWidth: 180
                Layout.alignment: Qt.AlignVCenter
                text: "Frame Rate:"
            }

            Slider {
                id: fpsSlider
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignVCenter

                from: 1
                to: 120
                value: 30
            }

            Label {
                Layout.preferredWidth: 70
                Layout.alignment: Qt.AlignVCenter
                text: Math.round(fpsSlider.value) + " FPS"
            }
        }
    }
}
