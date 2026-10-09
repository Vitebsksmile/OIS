import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQml.Models

Page {
    id: root

    signal closeRequested()

    title: "Settings"

    // Main background layer
    background: Rectangle {
        color: Theme.cBg
        radius: 16
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        RowLayout {
            // --- PAGE TITLE ---
            TitleText {
                text: root.title
                Layout.fillWidth: true

                //  Bottom title line
                Rectangle {
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: -8
                    width: parent.width
                    height: 2
                    color: Theme.cBorder
                }
            }

            Button {
                Layout.preferredWidth: implicitHeight
                text: "\u2715"
                icon.name: "exit"

                onClicked: root.closeRequested()
            }
        }

        // Небольшой компенсационный отступ под линией заголовка
        Item { Layout.preferredHeight: 5 }

        ListView {
            id: settingsList
            Layout.minimumWidth: 300
            Layout.fillWidth: true
            Layout.fillHeight: true
            rightMargin: 25
            spacing: 20

            interactive: true
            clip: true

            model: objectModel

            ScrollBar.vertical: ScrollBar {
                id: verticalScrollBar
                policy: settingsList.contentHeight > settingsList.height ? ScrollBar.AsNeeded : ScrollBar.AlwaysOff

                size: settingsList.visibleArea.heightRatio
                position: settingsList.visibleArea.yPosition
                active: settingsList.moving || settingsList.flicking || hovered

                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.rightMargin: -2

                onPositionChanged: {
                    if (pressed) {
                        settingsList.contentY = position * settingsList.contentHeight
                    }
                }
            }
        }
    }

    ObjectModel {
        id: objectModel

        CameraSettingsView {}
        ImageProcessingSettingsView {}
        MLSettingsView {}
        DatabaseSettingsView {}
    }
}

