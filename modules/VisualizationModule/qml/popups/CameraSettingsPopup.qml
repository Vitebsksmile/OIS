import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Popup {
    id: root
    parent: Overlay.overlay
    padding: 10
    x: Math.round((parent.width - width) / 2)
    y: Math.round((parent.height - height) / 2)

    signal popupClosed()

    onClosed: {
        popupClosed()
    }

    property string titlePopup: qsTr("USB device")

    //  Main background layer
    background: Rectangle {
        color: "lightgreen"
        radius: 16
    }

    contentItem: ColumnLayout {
        //anchors.fill: parent
        //anchors.margins: 20
        spacing: 15

        TitleText {
            Layout.fillWidth: true
            text: root.titlePopup

            // Нижняя линия заголовка
            Rectangle {
                anchors.bottom: parent.bottom
                //anchors.bottomMargin: -8
                width: parent.width
                height: 2
                color: Theme.cBorder
            }
        }

        // Отступ после разделителя
        Item { implicitHeight: 5 }

        NormalText {
            Layout.fillWidth: true
            //text: root.errorDetails
        }

        StackView {
            id: mainStack
            Layout.preferredWidth: 600
            Layout.preferredHeight: 200
            Layout.alignment: Qt.AlignHCenter
            Layout.margins: 20

            clip: true

            background: Rectangle {
                color: "black"
            }

            initialItem: usbCamera

            // ErrorTitlePopup {
            //     id: errorTitle
            //     errorTitle: root.errorTitle
            //     errorDetails: root.errorDetails
            // }
        }

        Button {
            Layout.preferredHeight: 30
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Close")
            highlighted: true
            onClicked: {
                root.close()
            }
        }

        // Отступ после разделителя
        Item { implicitHeight: 5 }
    }

    closePolicy: Popup.CloseOnEscape | Popup.NoAutoClose

    Component {
        id: usbCamera

        Page {
            //width: 200
            //height: 200
            background: Rectangle { color: "blue"; radius: 16 }

            RowLayout {
                anchors.centerIn: parent
                anchors.margins: 20
                spacing: 10

                //  Поле ввода логина
                TextField {
                    id: usernameField
                    Layout.preferredWidth: 200
                    Layout.alignment: Qt.AlignHCenter

                    color: activeFocus ? "black" : AuthController.existsByLogin(text) ? "black" : "red"
                    placeholderText: qsTr("Enter login")
                }

                LifeView {
                    id: processingView
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Layout.minimumWidth: 200
                    Layout.minimumHeight: 200

                    frameSource: "camera"
                }
            }
        }
    }
}
