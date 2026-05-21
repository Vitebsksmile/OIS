import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    //anchors.fill: parent
    color: "#f5f5f5"
    border.width: 20
    border.color: "#f0f0f0"
    anchors.bottomMargin: 10
    anchors.leftMargin: 5
    anchors.rightMargin: 5
    radius: 15

    implicitHeight: 150
    implicitWidth: 150

    ScrollView {
        anchors.fill: parent
        anchors.margins: 16

        ColumnLayout {
            id: rootLayout

            width: parent.width
            spacing: 20

            // ===== Заголовок =====
            Label {
                text: "System Settings"
                font.pixelSize: 28
                font.bold: true
                color: "#222"
            }

            // =====================================================
            // CAMERA SETTINGS
            // =====================================================

            Rectangle {
                Layout.fillWidth: true
                radius: 12

                color: "white"
                border.color: "#dcdcdc"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 14

                    Label {
                        text: "Camera Settings"
                        font.pixelSize: 22
                        font.bold: true
                    }

                    RowLayout {
                        Layout.fillWidth: true

                        Label {
                            text: "Camera Device:"
                            Layout.preferredWidth: 180
                        }

                        ComboBox {
                            Layout.fillWidth: true

                            model: [
                                "Default Camera",
                                "USB Camera #1",
                                "Industrial Camera"
                            ]
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true

                        Label {
                            text: "Resolution:"
                            Layout.preferredWidth: 180
                        }

                        ComboBox {
                            Layout.fillWidth: true

                            model: [
                                "640x480",
                                "1280x720",
                                "1920x1080"
                            ]
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true

                        Label {
                            text: "Frame Rate:"
                            Layout.preferredWidth: 180
                        }

                        Slider {
                            id: fpsSlider

                            Layout.fillWidth: true

                            from: 1
                            to: 120
                            value: 30
                        }

                        Label {
                            text: Math.round(fpsSlider.value) + " FPS"
                            Layout.preferredWidth: 70
                        }
                    }
                }
            }

            // =====================================================
            // IMAGE PROCESSING
            // =====================================================

            Rectangle {
                Layout.fillWidth: true
                radius: 12

                color: "white"
                border.color: "#dcdcdc"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 14

                    Label {
                        text: "Image Processing"
                        font.pixelSize: 22
                        font.bold: true
                    }

                    CheckBox {
                        text: "Enable Noise Reduction"
                        checked: true
                    }

                    CheckBox {
                        text: "Enable Contrast Enhancement"
                        checked: true
                    }

                    CheckBox {
                        text: "Enable Edge Detection"
                    }

                    RowLayout {
                        Layout.fillWidth: true

                        Label {
                            text: "Blur Radius:"
                            Layout.preferredWidth: 180
                        }

                        Slider {
                            id: blurSlider

                            Layout.fillWidth: true

                            from: 0
                            to: 20
                            value: 5
                        }

                        Label {
                            text: Math.round(blurSlider.value)
                            Layout.preferredWidth: 40
                        }
                    }
                }
            }

            // =====================================================
            // ML SETTINGS
            // =====================================================

            Rectangle {
                Layout.fillWidth: true
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

                        Label {
                            text: "Detection Threshold:"
                            Layout.preferredWidth: 180
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
                            text: thresholdSlider.value.toFixed(2)
                            Layout.preferredWidth: 50
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

            // =====================================================
            // DATABASE SETTINGS
            // =====================================================

            Rectangle {
                Layout.fillWidth: true
                radius: 12

                color: "white"
                border.color: "#dcdcdc"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 14

                    Label {
                        text: "Database Settings"
                        font.pixelSize: 22
                        font.bold: true
                    }

                    RowLayout {
                        Layout.fillWidth: true

                        Label {
                            text: "Database Path:"
                            Layout.preferredWidth: 180
                        }

                        TextField {
                            Layout.fillWidth: true
                            text: "database/inspection.db"
                        }

                        Button {
                            text: "Browse"
                        }
                    }

                    CheckBox {
                        text: "Enable Logging"
                        checked: true
                    }

                    CheckBox {
                        text: "Auto Save Results"
                        checked: true
                    }
                }
            }

            // =====================================================
            // BUTTONS
            // =====================================================

            RowLayout {
                Layout.alignment: Qt.AlignRight
                spacing: 12

                Button {
                    text: "Reset"

                    onClicked: {
                        console.log("Reset settings")
                    }
                }

                Button {
                    text: "Apply"

                    onClicked: {
                        console.log("Apply settings")
                    }
                }

                Button {
                    text: "Save"

                    highlighted: true

                    onClicked: {
                        console.log("Save settings")
                    }
                }
            }

            Item {
                Layout.fillHeight: true
                Layout.preferredHeight: 20
            }
        }
    }
}

