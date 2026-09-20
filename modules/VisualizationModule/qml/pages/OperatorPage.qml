import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: root
    title: "Operator"

    signal closeRequested()
    signal repairConfirmed()

    /// Main background layer
    background: Rectangle {
        color: Theme.cBg
        radius: 16
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        // --- ЗАГОЛОВОК СТРАНИЦЫ (.page-title) ---
        TitleText {
            text: root.title
            Layout.fillWidth: true

            // Нижняя линия заголовка
            Rectangle {
                anchors.bottom: parent.bottom
                anchors.bottomMargin: -8
                width: parent.width
                height: 2
                color: Theme.cBorder
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
                color: Theme.cDanger
                border.color: Theme.cBorderDanger
                border.width: 2
                radius: 6

                //  Видимость панели предупреждения зависит от модели
                visible: OperatorModel.hasAlarm

                // Анимация мигания (blink)
                SequentialAnimation on opacity {
                    loops: Animation.Infinite
                    NumberAnimation {
                        to: 0.6
                        duration: 1000
                        easing.type: Easing.InOutQuad }
                    NumberAnimation {
                        to: 1.0
                        duration: 1000
                        easing.type: Easing.InOutQuad }
                }

                TitleText {
                    anchors.centerIn: parent
                    text: OperatorModel.alarmMessage
                }
            }

            // 2. Текущий статус - Занимает 3 колонки
            Rectangle {
                Layout.columnSpan: 3
                Layout.fillWidth: true
                implicitHeight: 120
                color: Theme.cPanel
                border.color: Theme.cBorder
                radius: 6

                // Декоративная левая полоса (.border-left)
                Rectangle {
                    width: 5;
                    height: parent.height;
                    color: Theme.cPrimary
                    anchors.left: parent.left;
                    radius: 3
                }

                ColumnLayout {
                    anchors.fill: parent;
                    anchors.margins: 15;
                    spacing: 5
                    HeadingText {
                        text: "ТЕКУЩИЙ СТАТУС"
                    }

                    KpiText {
                        text: OperatorModel.currentStatus
                        color: Theme.cPrimary
                    }

                    Item { Layout.fillHeight: true }

                    MutedText {
                        text: "Оператор: " + OperatorModel.operatorName
                    }
                }
            }

            // 3. KPI FPY - Занимает 3 колонки
            Rectangle {
                Layout.columnSpan: 3
                Layout.fillWidth: true
                implicitHeight: 120
                color: Theme.cPanel
                border.color: Theme.cBorder
                radius: 6

                Rectangle {
                    width: 5;
                    height: parent.height
                    color: Theme.cSuccess
                    anchors.left: parent.left
                    radius: 3
                }

                ColumnLayout {
                    anchors.fill: parent;
                    anchors.margins: 15;
                    spacing: 5
                    HeadingText {
                        text: "FIRST PASS YIELD (FPY)"
                    }
                    KpiText {
                        text: OperatorModel.fpyValue
                        color: Theme.cSuccess
                    }
                    Item { Layout.fillHeight: true }
                    MutedText {
                        text: "Порог нормы: " + OperatorModel.fpyThreshold
                    }
                }
            }

            // 4. Счетчики смены - Занимает 6 колонок
            Rectangle {
                Layout.columnSpan: 6
                Layout.fillWidth: true
                implicitHeight: 120
                color: Theme.cPanel
                border.color: Theme.cBorder
                radius: 6

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15
                    spacing: 5
                    HeadingText {
                        text: "СЧЕТЧИКИ ЗА ТЕКУЩУЮ СМЕНУ";
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        Layout.topMargin: 10

                        Column {
                            Layout.fillWidth: true
                            NormalText {
                                text: "Проверено:"
                            }
                            KpiText {
                                text: OperatorModel.countTotal.toString()
                                color: Theme.cText
                            }
                        }
                        Column {
                            Layout.fillWidth: true
                            NormalText {
                                text: "Годны (Pass):"
                            }
                            KpiText {
                                text: OperatorModel.countPass.toString()
                                color: Theme.cSuccess
                            }
                        }
                        Column {
                            Layout.fillWidth: true
                            NormalText {
                                text: "Брак (Fail):"
                            }
                            KpiText {
                                text: OperatorModel.countFail.toString()
                                color: Theme.cTextDanger
                            }
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
                color: Theme.cPanel
                border.color: Theme.cBorder
                radius: 6

                ColumnLayout {
                    anchors.fill: parent;
                    anchors.margins: 15

                    HeadingText {
                        text: "ТЕКУЩАЯ ИНСПЕКЦИЯ: КАРТА ДЕФЕКТОВ ПЛАТЫ";
                    }

                    RowLayout {
                        //Layout.alignment: 0

                        //  Интерактивная карта
                        LifeView {
                            id: sourceViewer
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            Layout.minimumWidth: 200
                            Layout.minimumHeight: 200
                        }

                        //  Предобработанные кадры
                        ProcessingView {
                            id: processingView
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            Layout.minimumWidth: 200
                            Layout.minimumHeight: 200
                        }
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
                color: Theme.cPanel
                border.color: Theme.cBorder
                radius: 6

                ColumnLayout {
                    anchors.fill: parent;
                    anchors.margins: 15;
                    spacing: 10

                    HeadingText {
                        text: "ПОСЛЕДНИЙ ОБНАРУЖЕННЫЙ БРАК";
                    }

                    // Окно имитации камеры (.live-cam)
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "#374151"
                        radius: 4

                        Text {
                            anchors.centerIn: parent
                            text: OperatorModel.lastDefectPhoto
                                  + "\nПозиция: "
                                  + OperatorModel.lastDefectPosition
                                  + "\nДефект: "
                                  + OperatorModel.lastDefectType

                            color: "white"
                            font.family: Theme.fFamily
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
                        // Text {
                        //     text: "Подтвердить / Отправить в ремонт"
                        //     color: Theme.cText
                        //     font.family: Theme.fFamily
                        //     font.pixelSize: 14
                        //     font.bold: true
                        //     horizontalAlignment: Text.AlignHCenter
                        //     verticalAlignment: Text.AlignVCenter
                        // }
                        NormalText {
                            text: "Подтвердить / Отправить в ремонт"
                            color: Theme.cTextDanger
                        }

                        // background: Rectangle {
                        //     color: repairBtn.pressed ? "#D97706" : (repairBtn.hovered ? "#F59E0B" : "#FBBF24")
                        //     radius: 4
                        // }

                        // contentItem: Text {
                        //     text: "Подтвердить / Отправить в ремонт"
                        //     color: Theme.cText
                        //     font.family: Theme.fFamily
                        //     font.pixelSize: 14
                        //     font.bold: true
                        //     horizontalAlignment: Text.AlignHCenter
                        //     verticalAlignment: Text.AlignVCenter
                        // }

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