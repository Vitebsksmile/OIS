import QtQuick
import QtQuick.Controls 2.5
import QtQuick.Layouts
pragma ComponentBehavior: Bound

Popup {
    id: root

    property string titlePopup: qsTr("USB device")
    property string errorTitle: ""
    property string errorDetails: ""
    property int componentSpacing: 10

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
    background: Rectangle { color: Theme.cBg; radius: 12 }

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

        RowLayout {
            Layout.fillWidth: true
            Layout.margins: 20
            spacing: 10

            ColumnLayout {
                Layout.preferredWidth: implicitWidth
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignHCenter
                spacing: 0

                TabBar {
                    id: tabBar
                    Layout.preferredWidth: 400
                    Layout.preferredHeight: implicitHeight
                    Layout.alignment: Qt.AlignHCenter

                    TabButton {
                        text: qsTr("USB camera")

                        onClicked: {
                            root.titlePopup = text
                        }
                    }
                    TabButton {
                        text: qsTr("URL camera")

                        onClicked: {
                            root.titlePopup = text
                        }
                    }
                }

                StackLayout {
                    id: stackLayout
                    Layout.preferredWidth: 400
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignHCenter

                    currentIndex: tabBar.currentIndex
                    Loader {
                        id: usbCameraLoader
                        active: tabBar.currentIndex === 0
                        sourceComponent: usbCamera
                    }
                    Loader {
                        id: urlCameraLoader
                        active: tabBar.currentIndex === 1
                        sourceComponent: urlCamera
                    }
                }
            }

            LifeView {
                id: cameraView
                Layout.fillWidth: true
                Layout.fillHeight: true

                Layout.minimumWidth: 200
                Layout.minimumHeight: 200

                radius: 12

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
            padding: 10
            background: Rectangle { color: "white"; border.color: Theme.cBorder; bottomLeftRadius: 12; bottomRightRadius: 12 }

            ColumnLayout {
                anchors.centerIn: parent
                anchors.margins: 20
                spacing: 10

                OISNormalLabel {
                    text: qsTr("Input camera index:")
                    color: Theme.cText
                }

                Row {
                    spacing: root.componentSpacing

                    //  Port input field
                    OISNumberField {
                        id: indexField

                        width: 120
                        placeholderText: "00"

                        inputMethodHints: Qt.ImhDigitsOnly
                        validator: RegularExpressionValidator {
                            // Разрешает только одну или две цифры (0-99)
                            regularExpression: /^\d{1,2}$/
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

                        property int cameraIndex: Number(indexField.text)
                        //highlighted: true
                        onClicked: {
                            if (!SessionController.creatCamera(cameraIndex)) {
                                console.log(root.errorTitle)
                            } else {
                                console.log(root.errorTitle)
                            }
                        }
                    }
                }


            }
        }
    }

    Component {
        id: urlCamera

        Page {
            //width: 200
            //height: 200
            padding: 10
            background: Rectangle { color: "white"; border.color: Theme.cBorder; bottomLeftRadius: 12; bottomRightRadius: 12 }

            ColumnLayout {
                anchors.centerIn: parent
                anchors.margins: 20
                spacing: 10

                OISNormalLabel {
                    text: qsTr("Input IP address and Port number:")
                    color: Theme.cText
                }

                Row {
                    spacing: 0

                    OISNumberField {
                        id: urlField

                        width: 120
                        placeholderText: "000.000.000.000"

                        inputMethodHints: Qt.ImhDigitsOnly
                        validator: RegularExpressionValidator {
                            // 1. Шаблон для одного октета IP (0-255)
                            readonly property string octet: "(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)"

                            // Собираем регулярное выражение динамически для читаемости
                            regularExpression: new RegExp("^" + octet + "(\\." + octet + "){0,3}")
                        }
                        onAccepted: portField.forceActiveFocus()
                    }

                    Label {
                        id: separator
                        width: contentWidth + leftPadding + rightPadding
                        height: parent.height

                        horizontalAlignment: TextInput.AlignHCenter
                        verticalAlignment: Text.AlignVCenter

                        // background: Rectangle{
                        //     color: "white"
                        // }

                        text: ":"
                        font.family: Theme.fFamily
                        font.pixelSize: Theme.fSizeNormal
                        font.bold: Theme.fBoldNormal

                        color: portField.activeFocus ? palette.highlight : palette.text
                        Behavior on color { ColorAnimation { duration: 150 } }
                    }

                    //  Port input field
                    OISNumberField {
                        id: portField

                        width: 120
                        placeholderText: "from 0 to 65535"

                        inputMethodHints: Qt.ImhDigitsOnly
                        validator: RegularExpressionValidator {
                            regularExpression: /^([0-5]?\d{1,4}|6[0-4]\d{3}|65[0-4]\d{2}|655[0-2]\d|6553[0-5])$/
                        }
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


