import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// =====================================================
// ML SETTINGS
// =====================================================

Rectangle {
    anchors.fill: parent
    radius: 12

    color: "white"
    border.color: "#dcdcdc"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 14

        Label {
            text: "Machine Learning Settings"
            font.pixelSize: 22
            font.bold: true
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 30

            Label {
                Layout.preferredWidth: 180
                text: "Detection Threshold:"
            }

            Slider {
                id: thresholdSlider

                Layout.fillWidth: true

                from: 0
                to: 1
                stepSize: 0.01
                value: 0.75
            }

            Label {
                Layout.preferredWidth: 50
                text: thresholdSlider.value.toFixed(2)
            }
        }

        CheckBox {
            text: "Enable GPU Acceleration"
            checked: true
        }

        CheckBox {
            text: "Save Detection Results"
            checked: true
        }

        RowLayout {
            Layout.fillWidth: true

            Label {
                text: "Model:"
                Layout.preferredWidth: 180
            }

            ComboBox {
                Layout.fillWidth: true

                model: [
                    "U-Net",
                    "ResNet",
                    "MobileNet",
                    "YOLO"
                ]
            }
        }
    }
}
