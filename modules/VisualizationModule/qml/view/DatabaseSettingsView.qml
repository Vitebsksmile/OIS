import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// =====================================================
// DATABASE SETTINGS
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
            text: "Database Settings"
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
                text: "Database Path:"
            }

            TextField {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignVCenter
                text: "database/inspection.db"
            }

            Button {
                text: "Browse"
            }
        }

        CheckBox {
            Layout.preferredHeight: implicitHeight
            Layout.alignment: Qt.AlignVCenter
            text: "Enable Logging"
            checked: true
        }

        CheckBox {
            Layout.preferredHeight: implicitHeight
            Layout.alignment: Qt.AlignVCenter
            text: "Auto Save Results"
            checked: true
        }
    }
}
