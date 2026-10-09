import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// =====================================================
// ML SETTINGS
// =====================================================

Rectangle {
    width: ListView.view ? (ListView.view.width - ListView.view.rightMargin) : 0
    implicitHeight: rootLayout.implicitHeight + 32
    radius: 12

    color: "white"
    border.color: "#dcdcdc"

    RowLayout {
        id: rootLayout
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.topMargin: 16
        spacing: 10

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 10

            RowLayout {
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: implicitHeight

                OISTitleLabel {
                    Layout.preferredHeight: implicitHeight
                    Layout.alignment: Qt.AlignVCenter
                    text: "Machine Learning Settings"
                }

                Switch {
                    Layout.preferredHeight: implicitHeight
                    Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                    text: qsTr("Enable detection")
                    LayoutMirroring.enabled: true
                    checked: false
                    onCheckedChanged: {
                        if (checked) {
                            SessionController.changesDetectionMethod("yolo11")
                            console.log("Переключатель ВКЛЮЧЕН")
                        } else {
                            console.log("Переключатель ВЫКЛЮЧЕН")
                        }
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.preferredHeight: 30
                Layout.alignment: Qt.AlignVCenter

                OISNormalLabel {
                    Layout.preferredWidth: 180
                    text: "Detection Threshold:"
                }

                Slider {
                    id: thresholdSlider

                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignVCenter

                    from: 0
                    to: 1
                    stepSize: 0.01
                    value: 0.75
                }

                OISNormalLabel {
                    Layout.preferredWidth: 50
                    Layout.alignment: Qt.AlignVCenter
                    text: thresholdSlider.value.toFixed(2)
                }
            }

            OISNormalCheckBox {
                text: "Enable GPU Acceleration"
                checked: true
            }

            OISNormalCheckBox {
                text: "Save Detection Results"
                checked: true
            }

            RowLayout {
                Layout.fillWidth: true

                OISNormalLabel {
                    Layout.preferredWidth: 180

                    text: "Model:"
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

        LifeView {
            id: cameraView
            Layout.preferredWidth: 200
            Layout.fillHeight: true

            radius: 12

            frameSource: "yolo11"
        }
    }
}
