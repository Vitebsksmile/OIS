import QtQuick          //  Базовые элементы (Rectangle, Image, Text)
import QtQuick.Controls //  Стандартные элементы (Window, Button, Popup, Menu, SplitView)


pragma ComponentBehavior: Bound


ApplicationWindow {

    id: window

    visible: true

    color: "#526d80"

    width: 1200; height: 860

    menuBar: AppMenuBar {}

    HomeView {}

}
