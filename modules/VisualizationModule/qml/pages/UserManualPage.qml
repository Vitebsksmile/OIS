import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: root
    title: "User Manual"

    signal closeRequested()

    /// Main background layer
    background: Rectangle {
        color: Theme.cBg
        radius: 16
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        RowLayout {
            // --- ЗАГОЛОВОК СТРАНИЦЫ (.page-title) ---
            TitleText {
                text: root.title
                Layout.fillWidth: true

                // Нижняя линия заголовка
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
                icon.name: "close"

                onClicked: root.closeRequested()
            }
        }

        // Отступ после разделителя
        Item { implicitHeight: 5 }

        Rectangle {
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: parent.height
            color: "black"
        }
    }
}
