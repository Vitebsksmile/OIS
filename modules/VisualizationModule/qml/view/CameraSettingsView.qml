import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// =====================================================
// CAMERA SETTINGS
// =====================================================

Rectangle {
    width: ListView.view ? (ListView.view.width - ListView.view.rightMargin) : 0
    implicitHeight: rootLayout.implicitHeight + 32
    radius: 12

    color: "white"
    border.color: Theme.cBorder

    Loader {
        id: mainLoader
        active: false
        //anchors.fill: parent
        source: "../popups/CameraSettingsPopup.qml"

        onLoaded: {
            mainLoader.item.popupClosed.connect(function() {
                mainLoader.active = false
            })
            item.open()
        }
    }

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

            OISTitleLabel {
                Layout.preferredHeight: implicitHeight
                Layout.alignment: Qt.AlignVCenter
                text: qsTr("Camera Settings")
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.preferredHeight: implicitHeight
                Layout.alignment: Qt.AlignVCenter

                Button {
                    id: connectButton
                    Layout.preferredWidth: 180
                    Layout.alignment: Qt.AlignVCenter
                    text: qsTr("Connect a new camera")
                    onClicked: {
                        mainLoader.active = true
                    }
                }

                Button {
                    id: disconnectButton
                    Layout.preferredWidth: 180
                    Layout.alignment: Qt.AlignVCenter
                    text: qsTr("Disconnect the camera")
                    onClicked: {
                        SessionController.disconnectCamera()
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.preferredHeight: implicitHeight
                Layout.alignment: Qt.AlignVCenter

                OISNormalLabel {
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

                OISNormalLabel {
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

                OISNormalLabel {
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

                OISNormalLabel {
                    Layout.preferredWidth: 70
                    Layout.alignment: Qt.AlignVCenter
                    text: Math.round(fpsSlider.value) + " FPS"
                }
            }
        }

        LifeView {
            id: cameraView
            Layout.preferredWidth: 200
            Layout.fillHeight: true

            radius: 12

            frameSource: "camera"
        }
    }
}
