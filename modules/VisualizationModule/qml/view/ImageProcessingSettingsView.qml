import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// =====================================================
// IMAGE PROCESSING
// =====================================================

Rectangle {
    width: ListView.view ? (ListView.view.width - ListView.view.rightMargin) : 0
    implicitHeight: rootLayout.implicitHeight + 32
    radius: 12

    color: "white"
    border.color: "#dcdcdc"

    ColumnLayout {
        id: rootLayout
        anchors.left: parent.left
        anchors.right: parent.right
        //anchors.margins: 16
        anchors.top: parent.top
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.topMargin: 16
        spacing: 14

        OISTitleLabel {
            Layout.preferredHeight: implicitHeight
            Layout.alignment: Qt.AlignVCenter
            text: "Image Processing"
        }

        OISNormalCheckBox {
            Layout.preferredHeight: implicitHeight
            Layout.alignment: Qt.AlignVCenter
            text: "Enable Noise Reduction"
            checked: true
        }

        OISNormalCheckBox {
            Layout.preferredHeight: implicitHeight
            Layout.alignment: Qt.AlignVCenter
            text: "Enable Contrast Enhancement"
            checked: true
        }

        OISNormalCheckBox {
            Layout.preferredHeight: implicitHeight
            Layout.alignment: Qt.AlignVCenter
            text: "Enable Edge Detection"
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: implicitHeight

            OISNormalLabel {
                Layout.preferredWidth: 180
                Layout.preferredHeight: implicitHeight
                Layout.alignment: Qt.AlignVCenter
                text: "Blur Radius:"
            }

            Slider {
                id: blurSlider
                Layout.fillWidth: true
                Layout.preferredHeight: implicitHeight
                Layout.alignment: Qt.AlignVCenter

                from: 0
                to: 20
                value: 5
            }

            OISNormalLabel {
                Layout.preferredWidth: 40
                Layout.preferredHeight: implicitHeight
                Layout.alignment: Qt.AlignVCenter
                text: Math.round(blurSlider.value)
            }
        }
    }
}