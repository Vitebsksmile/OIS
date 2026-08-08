import QtQuick          //  Базовые элементы (Rectangle, Image, Text)
import QtQuick.Controls //  Стандартные элементы (Window, Button, Popup, Menu, SplitView)


pragma ComponentBehavior: Bound


ApplicationWindow {

    id: window

    readonly property Drawer mainMenu: menu

    visible: true

    //color: "#526d80"

    width: 1200
    height: 700

    minimumWidth: 500
    minimumHeight: 500

    menuBar: AppMenuBar {
        id: appMenuBar

        width: parent.width
        anchors.top: parent.top
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        anchors.topMargin: 0
        anchors.bottomMargin: 5
    }

    Drawer {
        id: menu
        width: 250
        height: parent.height

        ListView {
            anchors.fill: parent
            model: ["Dashboard", "Inspection", "Statistics", "Operator"]    //, "Dataset", "Settings"

            delegate: ItemDelegate {
                required property string modelData

                text: modelData
                onClicked: {
                    stackView.push("qml/pages/" + modelData + "Page.qml")
                    menu.close()
                }
            }
        }
    }

    Item {

        id: customHeaderContainer

        width: parent.width
        height: appToolBar.implicitHeight + anchors.topMargin + anchors.bottomMargin

        anchors.top: parent.top
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        anchors.topMargin: 0
        anchors.bottomMargin: 5

        AppToolBar {
            id: appToolBar
            onMenuTriggered: menu.open()
        }

    }

    StackView {
        id: stackView

        anchors.top: customHeaderContainer.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        anchors.leftMargin: 5
        anchors.rightMargin: 5
        anchors.topMargin: 5
        anchors.bottomMargin: 15

        /*background: Rectangle {
            color: "#526d80"
        }*/

        initialItem: "qml/pages/OperatorPage.qml"
    }

}
