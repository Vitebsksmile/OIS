import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: statisticsPage

    title: qsTr("Statistics")

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

            // ===== Центральная часть =====
            RowLayout {
                Layout.fillWidth: true
                //Layout.fillHeight: true
                Layout.preferredHeight: 100
                spacing: 5

                // ===== Верхняя панель метрик =====
                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: 2

                    Rectangle {
                        Layout.preferredWidth: 70
                        Layout.fillHeight: true

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
                        Layout.preferredWidth: 70
                        Layout.fillHeight: true

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
                        Layout.preferredWidth: 70
                        Layout.fillHeight: true

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

                // ===== График =====
                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    radius: 12
                    color: "white"
                    border.color: "#dcdcdc"

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 5

                        //var

                        Label {
                            padding: 2.5
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

                            // Приказываем Canvas перерисовываться с нуля при изменении размеров
                            onWidthChanged: requestPaint()
                            onHeightChanged: requestPaint()

                            onPaint: {
                                var ctx = getContext("2d")
                                ctx.reset()

                                var w = width
                                var h = height

                                if (w <= 0 || h <= 0) return; // Защита от нулевых размеров

                                // --- НАСТРОЙКА РАЗМЕРА ГРАФИКА (Увеличьте числа, чтобы сделать график МЕНЬШЕ) ---
                                var paddingTop = 5
                                var paddingBottom = 10
                                var paddingLeft = 5
                                var paddingRight = 5

                                // Автоматический расчет доступной ширины и высоты для рисования
                                var availableHeight = h - paddingTop - paddingBottom
                                var availableWidth = w - paddingLeft - paddingRight

                                // фон
                                ctx.fillStyle = "#ffffff"
                                ctx.fillRect(0, 0, w, h)

                                // оси (теперь строятся по динамическим отступам)
                                ctx.strokeStyle = "#999"
                                ctx.lineWidth = 2

                                ctx.beginPath()
                                ctx.moveTo(paddingLeft, paddingTop)
                                ctx.lineTo(paddingLeft, h - paddingBottom)
                                ctx.lineTo(w - paddingRight, h - paddingBottom)
                                ctx.stroke()

                                // сетка
                                ctx.strokeStyle = "#eeeeee"
                                ctx.lineWidth = 1

                                for (var i = 0; i < 5; i++) {
                                    // Теперь сетка и график используют одинаковый availableHeight
                                    var gy = paddingTop  + i * (availableHeight / 4)

                                    ctx.beginPath()
                                    ctx.moveTo(paddingLeft, gy)
                                    ctx.lineTo(w - paddingRight, gy)
                                    ctx.stroke()
                                }

                                // линия графика
                                var stepX = availableWidth  / (values.length - 1)

                                ctx.strokeStyle = "#1976d2"
                                ctx.lineWidth = 3

                                ctx.beginPath()

                                for (var j = 0; j < values.length; j++) {

                                    var x = paddingLeft  + j * stepX

                                    // Координата Y теперь рассчитывается синхронно с сеткой
                                    var y = (h - paddingBottom) - (values[j] / maxValue) * availableHeight

                                    if (j === 0)
                                        ctx.moveTo(x, y)
                                    else
                                        ctx.lineTo(x, y)
                                }

                                ctx.stroke()

                                // точки
                                ctx.fillStyle = "#d32f2f"

                                for (var k = 0; k < values.length; k++) {

                                    var px = paddingLeft + k * stepX

                                    var py = (h - paddingBottom) - (values[k] / maxValue) * availableHeight

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
                        anchors.margins: 5
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
