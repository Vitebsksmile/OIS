import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// =====================================================
// BUTTONS
// =====================================================

RowLayout {
    anchors.fill: parent
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
