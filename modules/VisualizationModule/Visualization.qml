import QtQuick          //  Базовые элементы (Rectangle, Image, Text)
import QtQuick.Controls //  Стандартные элементы (Window, Button, Popup, Menu, SplitView)

pragma ComponentBehavior: Bound

ApplicationWindow {
    id: window

    visible: true

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

    Loader {
        id: mainLoader
        anchors.fill: parent
        source: "qml/pages/LoginRegistrationPage.qml"
        property string previousPage: ""

        Connections {
            target: AuthController
            function onAuthenticationSuccess() {
                mainLoader.source = "qml/pages/OperatorPage.qml"
            }
            function onAuthFailed(error) {
                if (mainLoader.item && mainLoader.item.hasOwnProperty("errorMessage")) {
                    mainLoader.item.errorMessage = error
                }
            }
        }
    }

    Connections {
        target: appMenuBar
        function onMenuItemClicked(pathToPage) {
            mainLoader.previousPage = mainLoader.source.toString()
            mainLoader.source = pathToPage
        }
    }

    Connections {
        target: mainLoader.item
        function onCloseRequested() {
            if (mainLoader.source.toString() !== mainLoader.previousPage.toString()) {
                console.log("------if{}")
                console.log(mainLoader.source + " is closed")
                mainLoader.source = mainLoader.previousPage
            } else {
                console.log("------else{}")
                mainLoader.source = ""
            }
        }
    }

    // Item {

    //     id: customHeaderContainer

    //     width: parent.width
    //     height: appToolBar.implicitHeight + anchors.topMargin + anchors.bottomMargin

    //     anchors.top: parent.top
    //     anchors.leftMargin: 5
    //     anchors.rightMargin: 5
    //     anchors.topMargin: 0
    //     anchors.bottomMargin: 5

    //     AppToolBar {
    //         id: appToolBar
    //         onMenuTriggered: menu.open()
    //     }

    // }

    // StackView {
    //     id: stackView

    //     anchors.top: customHeaderContainer.bottom
    //     anchors.bottom: parent.bottom
    //     anchors.left: parent.left
    //     anchors.right: parent.right

    //     anchors.leftMargin: 5
    //     anchors.rightMargin: 5
    //     anchors.topMargin: 5
    //     anchors.bottomMargin: 15

    //     initialItem: "qml/pages/SettingsPage.qml"
    // }

}
