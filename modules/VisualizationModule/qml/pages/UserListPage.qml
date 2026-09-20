import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import VisualizationModule

pragma ComponentBehavior: Bound

Page {
    id: root
    title: "Operators List"

    signal closeRequested()

    //property var operatorModel: DbModelController.dbModel("operators")
    property var operatorModel: DbModelController.abstractTableModel("operators")
    // Создаем массив с названиями колонок для отображения в шапке
    readonly property var headerTitles: ["ID Оператора", "Логин", "ФИО Сотрудника", "Пароль"]
    // readonly property var headerTitles: operatorModel.tableName() ???????

    // Main background layer
    background: Rectangle {
        color: Theme.cBg
        radius: 16
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        RowLayout {
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

            Button {
                Layout.preferredWidth: implicitHeight
                text: "\u2715"
                icon.name: "close"

                onClicked: root.closeRequested()
            }
        }

        // Отступ после разделителя
        Item { implicitHeight: 5 }

        // Обернем таблицу в ScrollView, чтобы работала прокрутка
        ScrollView {
            Layout.fillWidth: parent.width
            // Разрешаем прокрутку контента по вертикали и горизонтали
            contentWidth: parent.width
            contentHeight: parent.height
            clip: true

            Column {
                id: mainColumn

                //  Ширина колонки должна быть равна сумме ширин всех столбцов таблицы
                width: operatorsTable.contentWidth
                height: root.height
                spacing: 0

                //  Step one: Add a horizontal header for the table
                HorizontalHeaderView {
                    id: tableHeader
                    width: parent.width
                    height: 35
                    //  Синхронизируем шапку с таблицей
                    syncView: operatorsTable
                    boundsBehavior: Flickable.StopAtBounds

                    //  Настраиваем внешний вид ячеек шапки
                    delegate: Rectangle {
                        id: headerCell

                        // 🌟 ИСПРАВЛЕНИЕ: Ширина ячейки шапки зависит от длины заголовка + отступы
                        implicitWidth: headerText.implicitWidth + 30
                        implicitHeight: 35
                        color: "#e2e8f0"
                        border.color: "#cbd5e1"

                        //  Number of the current header column
                        required property int column

                        Text {
                            id: headerText
                            anchors.centerIn: parent

                            //  Выводим текст из массива по индексу колонки
                            text: root.headerTitles[headerCell.column] !== undefined ? root.headerTitles[headerCell.column] : ""
                            font.bold: true
                            font.pointSize: 11
                            color: "#1e293b"
                        }
                    }
                }

                TableView {
                    id: operatorsTable

                    width: parent.width
                    height: parent.height - tableHeader.height
                    columnSpacing: 1
                    rowSpacing: 1
                    boundsBehavior: Flickable.StopAtBounds

                    // Подключаем вашу C++ модель из DbModelController
                    //model: DbModelController.dbModel("operators")
                    model: DbModelController.abstractTableModel("operators")

                    //  Настройка ширины колонок (в пикселях)
                    columnWidthProvider: function (column) {

                        //  Получаем неявную ширину на основе содержимого самой широкой ячейки в этой колонке
                        var tableContentWidth = operatorsTable.implicitColumnWidth(column)
                        var headerContentWidth = tableHeader.implicitColumnWidth(column)
                        var maxContentWidth = Math.max(tableContentWidth, headerContentWidth)

                        // Задаем минимальную ширину (например, 80px), чтобы колонка не схлопывалась, если она пустая
                        var minWidth = 80

                        return Math.max(maxContentWidth, minWidth)
                    }

                    // Делегат одной ячейки таблицы
                    delegate: Rectangle {
                        id: cell

                        //  Ширина ячейки теперь жестко привязана к ширине текста внутри неё + отступы
                        implicitWidth: cellText.implicitWidth + 30
                        implicitHeight: 40

                        // implicitWidth: 100
                        // implicitHeight: 40

                        // Мы требуем номер строки (row), номер колонки (column)
                        // и универсальный контейнер данных ячейки (modelData).
                        required property int row
                        required property int column
                        required property var modelData

                        //  Подсвечиваем первый столбец с ID, остальные оставляем белыми
                        color: cell.column === 0 ? "#f8fafc" : "white"
                        //border.color: "#e2e8а0"

                        Text {
                            id: cellText

                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: cell.column === 0 ? parent.horizontalCenter : undefined
                            anchors.left: cell.column === 0 ? undefined : parent.left
                            anchors.leftMargin: cell.column === 0 ? 0 : 12

                            font.bold: cell.column === 0
                            font.pointSize: 10

                            // 🌟 МАГИЯ ОТОБРАЖЕНИЯ:
                            // Если это колонка пароля (3) — выводим звездочки.
                            // Для остальных колонок (0, 1, 2) мы просто выводим modelData!
                            // В TableView движок Qt автоматически передает в modelData
                            // значение конкретно этой ячейки в зависимости от строки и столбца.

                            // Отображаем звездочки для пароля, для остальных — чистый текст ячейки
                            //text: cell.column === 3 ? "********" : (cell.modelData !== undefined ? cell.modelData : "")
                            text: cell.modelData
                            color: cell.column === 3 ? "#94a3b8" : "#334155"
                        }
                    }
                    Component.onCompleted: {
                        //console.log(DbModelController.logAbstractModel(model))
                        console.log(DbModelController.logQmlModelRoles(model))

                        //  Заставляем таблицу пересчитать геометрию колонок с учетом шапки
                        operatorsTable.forceLayout();
                    }
                }
            }
        }
    }
}
