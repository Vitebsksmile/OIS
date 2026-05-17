import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    anchors.fill: parent
    color: "#F3F3F3" // Базовый цвет Windows 11 Light

    // Сигнал сообщает главному окну, какой QML-файл загрузить
    signal modeSelected(string qmlFile)

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 30

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "Выберите режим работы"
            font.pixelSize: 22
            font.bold: true
            color: "#1A1A1A"
        }

        RowLayout {
            spacing: 20

            // Кнопка переключения на ImageView
            Button {
                id: imageBtn
                implicitWidth: 150; implicitHeight: 100
                text: "🖼️\nImageView"
                font.pixelSize: 14

                background: Rectangle {
                    color: imageBtn.hovered ? "#EAEAEA" : "#FFFFFF"
                    border.color: "#E5E5E5"
                    radius: 8
                }

                onClicked: root.modeSelected("ImageView.qml")
            }

            // Кнопка переключения на CameraView
            Button {
                id: cameraBtn
                implicitWidth: 150; implicitHeight: 100
                text: "📷\nCameraView"
                font.pixelSize: 14

                background: Rectangle {
                    color: cameraBtn.hovered ? "#EAEAEA" : "#FFFFFF"
                    border.color: "#E5E5E5"
                    radius: 8
                }

                onClicked: root.modeSelected("CameraView.qml")
            }
        }
    }
}
