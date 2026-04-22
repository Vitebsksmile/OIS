import QtQuick
import QtQuick.Controls 2.5
import QtQuick.Layouts


Rectangle {

    implicitHeight: internalLayout.implicitHeight + internalLayout.anchors.margins * 2

    color: Qt.rgba(44 / 255, 62 / 255, 80 / 255, 0.9) //  Midnight Blue
    radius: 10

    ColumnLayout {

        id: internalLayout

        //  Растягиваем по ширине
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        anchors.margins: 10 //  Внутренние отступы от краев родителя
        spacing: 5 //  Расстояние между кнопками

        Label {
            text: qsTr("Camcoder IP Address")
            color: "#ECF0F1" // Светло-серый, в стиле вашего Midnight Blue
            font.pixelSize: 12
            leftPadding: 2
        }

        TextField {
            id: ipField
            Layout.fillWidth: true

            // Стилизация под вашу темную тему
            background: Rectangle {
                implicitHeight: 35
                color: ipField.activeFocus ? "#34495E" : "#2C3E50"
                border.color: ipField.activeFocus ? "#3498DB" : "transparent"
                border.width: 2
                radius: 10
            }

            placeholderText: "192.168.1.100"
            placeholderTextColor: "white"

            color: "white"
            selectedTextColor: "white"
            selectionColor: "#3498DB"

            // Маска ввода (только цифры и точки)
            //inputMask: "000.000.0.000;_"

            // Центрируем текст по горизонтали и вертикали
            horizontalAlignment: TextInput.AlignHCenter
            verticalAlignment: TextInput.AlignVCenter

            // Сигнал по нажатию Enter
            onAccepted: {
                console.log("IP entered:", text)
            }
        }

    }

}
