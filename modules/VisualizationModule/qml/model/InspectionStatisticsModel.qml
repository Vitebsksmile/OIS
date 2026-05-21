import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// ===== Центральная часть. График =====
Rectangle {
    implicitWidth: layout.implicitWidth + (layout.anchors.margins * 2)
    implicitHeight: layout.implicitHeight + (layout.anchors.margins * 2)

    radius: 12
    color: "white"
    border.color: "#dcdcdc"

    ColumnLayout {
        id: layout
        anchors.fill: parent
        anchors.margins: 5
        spacing: 5

        Label {
            padding: 2.5
            text: qsTr("Inspection Statistics") //  Статистика проверок
            font.pixelSize: 18
            font.bold: true
        }

        Canvas {
            id: metricsCanvas

            Layout.fillWidth: true
            Layout.fillHeight: true

            Layout.minimumWidth: 200
            Layout.minimumHeight: 100

            property var values: [4, 7, 3, 10, 5, 8]

            // ШАГ 1: Перерисовываем холст при каждом изменении его размеров
            onWidthChanged: requestPaint()
            onHeightChanged: requestPaint()

            onPaint: {
                var ctx = getContext("2d");
                ctx.reset();

                // Если холст еще не инициализирован (размеры равны 0), выходим
                if (width <= 0 || height <= 0)
                    return;

                // --- НАСТРОЙКА РАЗМЕРА ГРАФИКА (Увеличьте числа, чтобы сделать график МЕНЬШЕ) ---
                var paddingTop = 5;
                var paddingBottom = 10;
                var paddingLeft = 5;
                var paddingRight = 5;

                // Автоматический расчет доступной ширины и высоты для рисования
                var availableHeight = height - paddingTop - paddingBottom;
                var availableWidth = width - paddingLeft - paddingRight;

                // Находим максимальное значение в массиве для правильного масштабирования по вертикали
                var maxVal = Math.max.apply(null, values);

                // фон
                ctx.fillStyle = "#ffffff";
                ctx.fillRect(0, 0, width, height);

                // оси
                ctx.strokeStyle = "#888";
                ctx.lineWidth = 2;

                ctx.beginPath();
                ctx.moveTo(paddingLeft, paddingTop);
                ctx.lineTo(paddingLeft, height - paddingBottom);
                ctx.lineTo(width - paddingRight, height - paddingBottom);
                ctx.stroke();

                // сетка
                ctx.strokeStyle = "#eeeeee";
                ctx.lineWidth = 1;

                for (var i = 0; i < 5; i++) {
                    // Теперь сетка и график используют одинаковый availableHeight
                    var gy = paddingTop + i * (availableHeight / 4);

                    ctx.beginPath();
                    ctx.moveTo(paddingLeft, gy);
                    ctx.lineTo(width - paddingRight, gy);
                    ctx.stroke();
                }

                // ШАГ 2: Динамический расчет шагов
                var stepX = availableWidth / (values.length - 1);

                // Функция для вычисления Y координат с учетом высоты экрана
                function getY(val) {
                    // Масштабируем значение относительно максимума и доступной высоты
                    return (height - paddingBottom) - (val / maxVal) * availableHeight;
                }

                // линия графика
                ctx.strokeStyle = "#1976d2";
                ctx.lineWidth = 3;

                ctx.beginPath();

                for (var i = 0; i < values.length; i++) {
                    var x = paddingLeft + i * stepX;
                    var y = getY(values[i]); // Используем динамический Y

                    if (i === 0)
                        ctx.moveTo(x, y);
                    else
                        ctx.lineTo(x, y);
                }

                ctx.stroke();

                // точки
                ctx.fillStyle = "#d32f2f";

                for (var j = 0; j < values.length; j++) {
                    var px = paddingLeft + j * stepX;
                    var py = getY(values[j]); // Используем динамический Y

                    ctx.beginPath();
                    ctx.arc(px, py, 5, 0, 2 * Math.PI);
                    ctx.fill();
                }
            }
        }
    }
}
