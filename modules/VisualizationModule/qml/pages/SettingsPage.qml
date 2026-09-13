import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQml.Models

Page {
    id: root
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

        // Небольшой компенсационный отступ под линией заголовка
        Item { Layout.preferredHeight: 5 }

        ListView {
            id: settingsList
            Layout.minimumWidth: 300
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 20

            //interactive: true
            clip: true

            model: objectModel

            ScrollBar.vertical: ScrollBar {
                id: verticalScrollBar
                policy: settingsList.contentHeight > settingsList.height ? ScrollBar.AsNeeded : ScrollBar.AlwaysOff
                anchors.right: settingsList.right
                anchors.rightMargin: -10 // Слегка отодвигаем к краю окна
            }
        }
    }

    ObjectModel {
        id: objectModel

        CameraSettingsView {}
        ImageProcessingSettingsView {}
        //MLSettingsView {}
        DatabaseSettingsView {}
    }
}

