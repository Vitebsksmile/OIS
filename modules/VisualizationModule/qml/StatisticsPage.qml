import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: statisticsPage

    title: qsTr("Statistics")

    //Layout.preferredHeight: 300
    implicitHeight: 150

    Rectangle {
        anchors.fill: parent
        color: "#f0f0f0"

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 2
            spacing: 5

            // ===== Заголовок =====
            Label {
                text: qsTr("Inspection Statistics")
                font.pixelSize: 18
                font.bold: true
                color: "#222"
            }

            // ===== Верхняя панель метрик =====
            RowLayout {
                Layout.fillWidth: true
                spacing: 5

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 70

                    radius: 12
                    color: "white"
                    border.color: "#dcdcdc"

                    Column {
                        anchors.centerIn: parent
                        spacing: 4

                        Label {
                            text: qsTr("Precision")
                            font.pixelSize: 15
                            color: "#666"
                        }

                        Label {
                            text: "0.972"
                            font.pixelSize: 18
                            font.bold: true
                            color: "#1976d2"
                        }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 70

                    radius: 10
                    color: "white"
                    border.color: "#dcdcdc"

                    Column {
                        anchors.centerIn: parent
                        spacing: 4

                        Label {
                            text: qsTr("Recall")
                            font.pixelSize: 15
                            color: "#666"
                        }

                        Label {
                            text: "0.948"
                            font.pixelSize: 18
                            font.bold: true
                            color: "#388e3c"
                        }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 70

                    radius: 10
                    color: "white"
                    border.color: "#dcdcdc"

                    Column {
                        anchors.centerIn: parent
                        spacing: 4

                        Label {
                            text: qsTr("F1-Score")
                            font.pixelSize: 15
                            color: "#666"
                        }

                        Label {
                            text: "0.960"
                            font.pixelSize: 18
                            font.bold: true
                            color: "#d32f2f"
                        }
                    }
                }
            }

            // ===== Центральная часть =====
            RowLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 16

                // ===== График =====
                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    radius: 12
                    color: "white"
                    border.color: "#dcdcdc"

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 12

                        Label {
                            text: qsTr("Detection Statistics")
                            font.pixelSize: 18
                            font.bold: true
                        }

                        Canvas {
                            id: statisticsCanvas

                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            property var values: [4, 7, 3, 10, 5, 8, 6]
                            property int maxValue: 12

                            onPaint: {
                                var ctx = getContext("2d")
                                ctx.reset()

                                var w = width
                                var h = height

                                // фон
                                ctx.fillStyle = "#ffffff"
                                ctx.fillRect(0, 0, w, h)

                                // оси
                                ctx.strokeStyle = "#999"
                                ctx.lineWidth = 2

                                ctx.beginPath()
                                ctx.moveTo(50, 20)
                                ctx.lineTo(50, h - 40)
                                ctx.lineTo(w - 20, h - 40)
                                ctx.stroke()

                                // сетка
                                ctx.strokeStyle = "#eeeeee"
                                ctx.lineWidth = 1

                                for (var i = 0; i < 5; i++) {
                                    var gy = 20 + i * ((h - 60) / 4)

                                    ctx.beginPath()
                                    ctx.moveTo(50, gy)
                                    ctx.lineTo(w - 20, gy)
                                    ctx.stroke()
                                }

                                // линия графика
                                var stepX = (w - 90) / (values.length - 1)

                                ctx.strokeStyle = "#1976d2"
                                ctx.lineWidth = 3

                                ctx.beginPath()

                                for (var j = 0; j < values.length; j++) {

                                    var x = 50 + j * stepX

                                    var y = (h - 40)
                                            - (values[j] / maxValue)
                                            * (h - 80)

                                    if (j === 0)
                                        ctx.moveTo(x, y)
                                    else
                                        ctx.lineTo(x, y)
                                }

                                ctx.stroke()

                                // точки
                                ctx.fillStyle = "#d32f2f"

                                for (var k = 0; k < values.length; k++) {

                                    var px = 50 + k * stepX

                                    var py = (h - 40)
                                            - (values[k] / maxValue)
                                            * (h - 80)

                                    ctx.beginPath()
                                    ctx.arc(px, py, 5, 0, 2 * Math.PI)
                                    ctx.fill()
                                }
                            }
                        }
                    }
                }

                // ===== Таблица результатов =====
                Rectangle {
                    Layout.preferredWidth: 420
                    Layout.fillHeight: true

                    radius: 12
                    color: "white"
                    border.color: "#dcdcdc"

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 10

                        Label {
                            text: qsTr("Recent Inspection Results")
                            font.pixelSize: 18
                            font.bold: true
                        }

                        ListView {
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            clip: true

                            model: [
                                "ID: 001 | " + qsTr("Scratch defect"),
                                "ID: 002 | " + qsTr("No defects"),
                                "ID: 003 | " + qsTr("Crack detected"),
                                "ID: 004 | " + qsTr("Surface contamination"),
                                "ID: 005 | " + qsTr("No defects"),
                                "ID: 006 | " + qsTr("Edge damage"),
                                "ID: 007 | " + qsTr("No defects")
                            ]

                            delegate: Rectangle {

                                id: delegateRoot

                                required property int index
                                required property string modelData

                                width: ListView.view.width
                                height: 20

                                radius: 6

                                color: delegateRoot.index % 2 === 0
                                       ? "#fafafa"
                                       : "#f0f0f0"

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
            }
        }
    }
}
