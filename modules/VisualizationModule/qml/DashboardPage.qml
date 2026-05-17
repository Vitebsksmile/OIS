import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


Page {
    id: dashboardPage

    title: qsTr("Dashboard")

    //Layout.preferredHeight: 350
    implicitHeight: 350

    Rectangle {
        anchors.fill: parent
        color: "#f0f0f0"

        RowLayout {
            anchors.fill: parent
            anchors.margins: 2
            spacing: 5

            // ===== Центральная часть =====
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true   //  min 290

                radius: 12
                color: "white"
                border.color: "#dcdcdc"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 5

                    Label {
                        text: qsTr("Inspection Statistics")
                        font.pixelSize: 18
                        font.bold: true
                    }

                    Canvas {
                        id: metricsCanvas

                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        property var values: [4, 7, 3, 10, 5, 8]

                        // ШАГ 1: Перерисовываем холст при каждом изменении его размеров
                        onWidthChanged: requestPaint()
                        onHeightChanged: requestPaint()

                        onPaint: {

                            var ctx = getContext("2d")
                            ctx.reset()

                            var w = width
                            var h = height

                            // фон
                            ctx.fillStyle = "#ffffff"
                            ctx.fillRect(0, 0, w, h)

                            //  Сжимаем всё содержимое холста (включая толщину линий осей и размер точек) в 1.5 раза относительно верхнего левого угла осей (40, 20)
                            //ctx.save()
                            // Сдвигаем начало координат к перекрестию осей, сжимаем всё в 0.3 раз (на 30%), и возвращаем назад
                            //ctx.translate(40, h - 40)
                            //ctx.scale(0.3, 0.3)
                            //ctx.translate(-40, -(h - 40))

                            // оси
                            ctx.strokeStyle = "#888"
                            ctx.lineWidth = 2

                            ctx.beginPath()
                            ctx.moveTo(40, 20)
                            ctx.lineTo(40, h - 40)
                            ctx.lineTo(w - 20, h - 40)
                            ctx.stroke()

                            // график
                            ctx.strokeStyle = "#1976d2"
                            ctx.lineWidth = 3

                            // линия графика
                            var stepX = (w - 80) / (values.length - 1)

                            ctx.beginPath()

                            for (var i = 0; i < values.length; i++) {

                                var x = 40 + i * stepX
                                var y = h - 40 - values[i] * 20

                                if (i === 0)
                                    ctx.moveTo(x, y)
                                else
                                    ctx.lineTo(x, y)
                            }

                            ctx.stroke()

                            // точки
                            ctx.fillStyle = "#d32f2f"

                            for (var j = 0; j < values.length; j++) {

                                var px = 40 + j * stepX
                                var py = h - 40 - values[j] * 20

                                ctx.beginPath()
                                ctx.arc(px, py, 5, 0, 2 * Math.PI)
                                ctx.fill()
                            }
                        }
                    }
                }
            }

            // ===== Верхняя панель статистики =====
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 10

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 30
                    radius: 12
                    color: "white"
                    border.color: "#dcdcdc"

                    Row {
                        anchors.centerIn: parent
                        spacing: 6

                        Label {
                            text: qsTr("Inspected Items")
                            font.pixelSize: 15
                            color: "#555"
                        }

                        Label {
                            text: "1248"
                            font.pixelSize: 18
                            font.bold: true
                        }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 30
                    radius: 12
                    color: "white"
                    border.color: "#dcdcdc"

                    Row {
                        anchors.centerIn: parent
                        spacing: 6

                        Label {
                            text: qsTr("Detected Defects")
                            font.pixelSize: 15
                            color: "#555"
                        }

                        Label {
                            text: "86"
                            font.pixelSize: 18
                            font.bold: true
                            color: "#d32f2f"
                        }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 30
                    radius: 12
                    color: "white"
                    border.color: "#dcdcdc"

                    Row {
                        anchors.centerIn: parent
                        spacing: 6

                        Label {
                            text: qsTr("Accuracy")
                            font.pixelSize: 15
                            color: "#555"
                        }

                        Label {
                            text: "97.8 %"
                            font.pixelSize: 18
                            font.bold: true
                            color: "#2e7d32"
                        }
                    }
                }
            }
        }
    }
}
