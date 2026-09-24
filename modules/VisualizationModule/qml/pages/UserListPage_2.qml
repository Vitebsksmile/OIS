import QtQuick
import QtQuick.Controls
pragma ComponentBehavior: Bound

Page {
    id: root
    title: "Operators List"

    signal closeRequested()
    property var tableModel: DbModelController.abstractTableModel("computers")

    HorizontalHeaderView {
        id: horizontalHeader
        anchors.left: tableView.left
        anchors.top: parent.top
        syncView: tableView
        clip: true
    }

    TableView {
        id: tableView
        anchors.left: parent.left
        anchors.top: horizontalHeader.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        model: root.tableModel
        columnSpacing: 1
        rowSpacing: 1

        delegate: Rectangle {
            id: cell
            implicitWidth: 150
            implicitHeight: 40
            border.width: 1

            required property var modelData

            Text {
                anchors.centerIn: parent
                text: cell.modelData
            }
        }
    }
}
