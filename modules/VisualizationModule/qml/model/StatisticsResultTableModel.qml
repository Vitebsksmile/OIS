import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// ===== Таблица результатов =====
Rectangle {
    implicitWidth: layout.implicitWidth + (layout.anchors.margins * 2)
    implicitHeight: layout.implicitHeight + (layout.anchors.margins * 2)

    radius: 10
    color: "white"
    border.color: "#dcdcdc"

    ColumnLayout {
        id: layout
        anchors.fill: parent
        anchors.margins: 10
        spacing: 5

        Label {
            text: qsTr("Recent Inspection Results")             //  Результаты последней проверки
            font.pixelSize: 18
            font.bold: true
        }

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            //Layout.preferredHeight: contentHeight

            clip: true

            //  ID обнаруженного элемента на изображении и описание деффекта или его отстутствие
            model: ["ID: 001 | " + qsTr("Scratch defect")          //  Дефект царапины
                , "ID: 002 | " + qsTr("No defects")              //  Без дефектов
                , "ID: 003 | " + qsTr("Crack detected")          //  Обнаружена трещина
                , "ID: 004 | " + qsTr("Surface contamination")   //  Загрязнение поверхности
                , "ID: 005 | " + qsTr("No defects"), "ID: 006 | " + qsTr("Edge damage")             //  Повреждения краев
                , "ID: 007 | " + qsTr("No defects")]

            delegate: Rectangle {
                id: delegateRoot

                required property int index
                required property string modelData

                width: ListView.view.width
                //height: ListView.view.height
                height: 20

                radius: 6

                color: delegateRoot.index % 2 === 0 ? "#fafafa" : "#f0f0f0"

                border.color: "#e0e0e0"

                Label {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 12

                    text: delegateRoot.modelData
                    font.pixelSize: 10
                    color: "#333"
                }
            }
        }
    }
}
