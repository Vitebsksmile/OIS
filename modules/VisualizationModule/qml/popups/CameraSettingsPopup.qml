import QtQuick
import QtQuick.Controls 2.5
import QtQuick.Layouts
pragma ComponentBehavior: Bound

Popup {
    id: root

    property string titlePopup: qsTr("USB device")
    property string errorTitle: ""
    property string errorDetails: ""

    signal popupClosed()

    closePolicy: Popup.CloseOnEscape | Popup.NoAutoClose

    parent: Overlay.overlay
    padding: 10
    x: Math.round((parent.width - width) / 2)
    y: Math.round((parent.height - height) / 2)

    modal: true //  Не блокирует взаимодействие с основным окном
    focus: false //  Не перехватывает ввод (Escape не сработает при false)

    onClosed: {
        popupClosed()
    }

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

        // NormalText {
        //     Layout.fillWidth: true
        //     //text: root.errorDetails
        // }

        RowLayout {
            Layout.fillWidth: true
            Layout.margins: 20
            spacing: 10

            StackView {
                id: mainStack
                Layout.preferredWidth: 400
                Layout.preferredHeight: 200
                Layout.alignment: Qt.AlignHCenter
                Layout.margins: 20

                clip: true

                background: Rectangle {
                    color: "black"
                }

                initialItem: usbCamera
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

    Component {
        id: usbCamera

        Page {
            //width: 200
            //height: 200
            background: Rectangle { color: "blue"; radius: 16 }

            ColumnLayout {
                anchors.centerIn: parent
                anchors.margins: 20
                spacing: 10

                Label {
                    text: qsTr("Input URL address and Port number")
                    color: "#ECF0F1" // Светло-серый, в стиле вашего Midnight Blue
                    font.pixelSize: 12
                    leftPadding: 2
                }

                Row {
                    //  URL input field
                    NumberField {
                        id: urlField

                        //color: activeFocus ? "black" : AuthController.existsByLogin(text) ? "black" : "red"
                        placeholderText: qsTr("Enter name")
                        inputMask: "000.000.000.00;_"
                        inputMethodHints: Qt.ImhDigitsOnly
                        validator: IntValidator { bottom: 1; top: 256 }
                    }

                    Label {
                        id: separator
                        width: contentWidth + leftPadding + rightPadding
                        height: parent.height

                        horizontalAlignment: TextInput.AlignHCenter
                        verticalAlignment: Text.AlignVCenter

                        background: Rectangle{
                            color: "white"
                        }

                        text: ":"
                        font.family: Theme.fFamily
                        font.pixelSize: Theme.fSizeNormal
                        font.bold: Theme.fBoldNormal
                    }

                    //  Port input field
                    NumberField {
                        id: portField

                        //color: activeFocus ? "black" : AuthController.existsByLogin(text) ? "black" : "red"
                        inputMask: "0000;_"
                        inputMethodHints: Qt.ImhDigitsOnly
                        validator: IntValidator { bottom: 1; top: 256 }
                    }
                }

                Button {
                    id: buttonIn
                    text: qsTr("Connection")
                    //Layout.preferredWidth: 100

                    // Используем встроенную системную иконку стрелки назад
                    //icon.name: "go-previous"
                    icon.height: 15
                    // Отображаем и иконку, и текст рядом
                    //display: AbstractButton.TextBesideIcon

                    property string url: urlField.text + separator.text + portField.text
                    //highlighted: true
                    onClicked: {
                        if (!SessionController.creatCamera(url)) {
                            console.log(root.errorTitle)
                        } else {
                            console.log(root.errorTitle)
                        }
                    }
                }
            }
        }
    }

    Connections {
        target: SessionController
        function onErrorOccurred(errorTitle, errorDetails) {
            root.errorTitle = errorTitle
            root.errorDetails = errorDetails
        }
    }

    ErrorTitlePopup {
        id: errorTitle
        errorTitle: root.errorTitle
        errorDetails: root.errorDetails
    }
}
