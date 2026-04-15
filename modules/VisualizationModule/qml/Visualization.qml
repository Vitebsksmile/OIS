import QtQuick          //  Базовые элементы (Rectangle, Image, Text)
import QtQuick.Controls //  Стандартные элементы (Window, Button, Popup, Menu, SplitView)


ApplicationWindow {

    id: window

    visible: true

    width: 1200; height: 860

    menuBar: AppMenuBar {}

    HomeView {}

    FutterView {}

}
