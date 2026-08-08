import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: root
    title: "Operator"

    // Палитра согласно CSS переменным (:root) шаблона
    readonly property color cBg: "#F4F6F9"
    readonly property color cPanel: "#FFFFFF"
    readonly property color cBorder: "#D1D5DB"
    readonly property color cText: "#1F2937"
    readonly property color cTextMuted: "#6B7280"
    readonly property color cPrimary: "#2563EB"
    readonly property color cSuccess: "#10B981"
    readonly property color cDanger: "#EF4444"
    readonly property color cWarning: "#F59E0B"

    // Сигналы для навигации
    signal settingsRequested()
    signal helpRequested()
    signal repairConfirmed()

    // Главная подложка фона
    Rectangle {
        anchors.fill: parent
        color: root.cBg
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15



        // --- ЗАГОЛОВОК СТРАНИЦЫ (.page-title) ---
        Text {
            text: "Оператор линии"
            font.pixelSize: 22
            font.bold: true
            color: root.cText
            Layout.fillWidth: true

            // Нижняя линия заголовка
            Rectangle {
                anchors.bottom: parent.bottom
                anchors.bottomMargin: -8
                width: parent.width
                height: 2
                color: root.cBorder
            }
        }

        // Отступ после разделителя
        Item { implicitHeight: 5 }

        // --- ОСНОВНАЯ СЕТКА (.grid) ---
        GridLayout {
            columns: 12
            rows: 3
            columnSpacing: 15
            rowSpacing: 15
            Layout.fillWidth: true
            Layout.fillHeight: true

            // 1. Панель предупреждения (.alarm) - Занимает 12 колонок
            Rectangle {
                Layout.columnSpan: 12
                Layout.fillWidth: true
                implicitHeight: 54
                color: "#FEE2E2"
                border.color: root.cDanger
                border.width: 2
                radius: 6

                // Анимация мигания (blink)
                SequentialAnimation on opacity {
                    loops: Animation.Infinite
                    NumberAnimation { to: 0.6; duration: 1000; easing.type: Easing.InOutQuad }
                    NumberAnimation { to: 1.0; duration: 1000; easing.type: Easing.InOutQuad }
                }

                Text {
                    anchors.centerIn: parent
                    text: "⚠️ СЕРИЙНЫЙ БРАК! Компонент R12 [Смещение] на 3-х платах подряд!"
                    color: "#991B1B"
                    font.pixelSize: 16
                    font.bold: true
                }
            }

            // 2. Текущий статус - Занимает 3 колонки
            Rectangle {
                Layout.columnSpan: 3
                Layout.fillWidth: true
                //Layout.fillHeight: true
                implicitHeight: 120
                color: root.cPanel
                border.color: root.cBorder
                radius: 6

                // Декоративная левая полоса (.border-left)
                Rectangle {
                    width: 5; height: parent.height; color: root.cPrimary
                    anchors.left: parent.left; radius: 3
                }

                ColumnLayout {
                    anchors.fill: parent; anchors.margins: 15; spacing: 5
                    Text { text: "ТЕКУЩИЙ СТАТУС"; font.pixelSize: 12; font.bold: true; color: root.cTextMuted }
                    Text { text: "РАБОТА"; font.pixelSize: 32; font.bold: true; color: root.cPrimary }
                    Item { Layout.fillHeight: true }
                    Text { text: "Оператор: Иванов И.И."; font.pixelSize: 12; color: root.cText }
                }
            }

            // 3. KPI FPY - Занимает 3 колонки
            Rectangle {
                Layout.columnSpan: 3
                Layout.fillWidth: true
                //Layout.fillHeight: true
                implicitHeight: 120
                color: root.cPanel
                border.color: root.cBorder
                radius: 6

                Rectangle {
                    width: 5; height: parent.height; color: root.cSuccess
                    anchors.left: parent.left; radius: 3
                }

                ColumnLayout {
                    anchors.fill: parent; anchors.margins: 15; spacing: 5
                    Text { text: "FIRST PASS YIELD (FPY)"; font.pixelSize: 12; font.bold: true; color: root.cTextMuted }
                    Text { text: "98.4 %"; font.pixelSize: 32; font.bold: true; color: root.cSuccess }
                    Item { Layout.fillHeight: true }
                    Text { text: "Порог нормы: >97.5%"; font.pixelSize: 12; color: root.cTextMuted }
                }
            }

            // 4. Счетчики смены - Занимает 6 колонок
            Rectangle {
                Layout.columnSpan: 6
                Layout.fillWidth: true
                //Layout.fillHeight: true
                implicitHeight: 120
                color: root.cPanel
                border.color: root.cBorder
                radius: 6

                ColumnLayout {
                    anchors.fill: parent; anchors.margins: 15; spacing: 5
                    Text { text: "СЧЕТЧИКИ ЗА ТЕКУЩУЮ СМЕНУ"; font.pixelSize: 12; font.bold: true; color: root.cTextMuted }

                    RowLayout {
                        Layout.fillWidth: true
                        Layout.topMargin: 10

                        Column {
                            Layout.fillWidth: true
                            Text { text: "Проверено:"; font.pixelSize: 13; color: root.cText }
                            Text { text: "1,245"; font.pixelSize: 24; font.bold: true; color: root.cText }
                        }
                        Column {
                            Layout.fillWidth: true
                            Text { text: "Годны (Pass):"; font.pixelSize: 13; color: root.cText }
                            Text { text: "1,225"; font.pixelSize: 24; font.bold: true; color: root.cSuccess }
                        }
                        Column {
                            Layout.fillWidth: true
                            Text { text: "Брак (Fail):"; font.pixelSize: 13; color: root.cText }
                            Text { text: "20"; font.pixelSize: 24; font.bold: true; color: root.cDanger }
                        }
                    }
                    Item { Layout.fillHeight: true }
                }
            }

            // 5. Карта дефектов (.pcb-map) - Занимает 8 колонок
            Rectangle {
                Layout.columnSpan: 8
                Layout.rowSpan: 2
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: root.cPanel
                border.color: root.cBorder
                radius: 6

                ColumnLayout {
                    anchors.fill: parent; anchors.margins: 15

                    Text { text: "ТЕКУЩАЯ ИНСПЕКЦИЯ: КАРТА ДЕФЕКТОВ ПЛАТЫ"; font.pixelSize: 12; font.bold: true; color: root.cTextMuted }

                    LifeView {
                        id: sourceViewer
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        Layout.minimumWidth: 200
                        Layout.minimumHeight: 200
                    }

                    /*
                    // Сама интерактивная карта (.pcb-map)
                    Rectangle {
                        id: pcbMap
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.topMargin: 10
                        color: "#065F46"
                        border.color: "#047857"
                        border.width: 2
                        radius: 6

                        Text {
                            anchors.centerIn: parent
                            text: "СХЕМА ПЕЧАТНОЙ ПЛАТЫ (LIVE)"
                            color: Qt.rgba(1, 1, 1, 0.3)
                            font.pixelSize: 16
                            font.bold: true
                        }

                        // Точка дефекта (.defect-dot)
                        Rectangle {
                            width: 20; height: 20
                            color: root.cDanger
                            border.color: "white"
                            border.width: 2
                            radius: 10

                            // Позиционирование в процентах (top: 40%, left: 65%)
                            x: pcbMap.width * 0.65 - radius
                            y: pcbMap.height * 0.40 - radius

                            // Свечение (Тень) точки дефекта
                            layer.enabled: true
                        }
                    }*/
                }
            }

            // 6. Последний обнаруженный брак - Занимает 4 колонки
            Rectangle {
                Layout.columnSpan: 4
                Layout.rowSpan: 2
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: root.cPanel
                border.color: root.cBorder
                radius: 6

                ColumnLayout {
                    anchors.fill: parent; anchors.margins: 15; spacing: 10

                    Text {
                        text:
                            "ПОСЛЕДНИЙ ОБНАРУЖЕННЫЙ БРАК";
                        font.pixelSize: 12; font.bold: true;
                        color: root.cTextMuted
                    }

                    // Окно имитации камеры (.live-cam)
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "#374151"
                        radius: 4

                        Text {
                            anchors.centerIn: parent
                            text: "[ ФОТО С КАМЕРЫ ]\nПозиция: R12\nДефект: Смещение компонента X/Y"
                            color: "white"
                            font.pixelSize: 13
                            lineHeight: 1.5
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }

                    // Кнопка подтверждения ремонтных операций (.btn)
                    Button {
                        id: repairBtn
                        Layout.fillWidth: true
                        implicitHeight: 40

                        background: Rectangle {
                            //color: repairBtn.pressed ? "#D97706" : (repairBtn.hovered ? "#F59E0B" : "#FBBF24")
                            radius: 4
                        }

                        contentItem: Text {
                            text: "Подтвердить / Отправить в ремонт"
                            color: root.cText
                            font.pixelSize: 14
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        onClicked: root.repairConfirmed()
                    }
                }
            }
        }
    }
}

/*
        // --- НАВИГАЦИЯ (.nav) ---
        Rectangle {
            Layout.fillWidth: true
            implicitHeight: 52
            color: "#1E293B"
            radius: 6

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 10
                anchors.rightMargin: 10

                Button {
                    id: btnSettings
                    text: "Настройки"
                    background: Rectangle {
                        color: btnSettings.hovered ? "#475569" : "#334155"
                        radius: 4
                    }
                    contentItem: Text {
                        text: btnSettings.text
                        color: "#CBD5E1"
                        font.pixelSize: 14
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: root.settingsRequested()
                }

                Item { Layout.fillWidth: true } // Разделитель (spacer)

                Button {
                    id: btnHelp
                    text: "Справка"
                    background: Rectangle {
                        color: btnHelp.hovered ? "#475569" : "#334155"
                        radius: 4
                    }
                    contentItem: Text {
                        text: btnHelp.text
                        color: "#CBD5E1"
                        font.pixelSize: 14
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: root.helpRequested()
                }
            }
        }
*/