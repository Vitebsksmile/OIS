import QtQuick
import QtQuick.Controls

//  Стандартные элементы (Window, Button, Popup, Menu, SplitView)

//  Правая часть окна (центральный контент)
SplitView {

    id: root

    //  Экспортируем внутренние объекты наружу,
    //  чтобы обращаться к ним как <id>.handler или <id>.popup
    property alias handler: sourceViewer.handler //  Текущий путь к изображению
    property alias popup: sourceViewer.popup //  Всплывающее уведомление

    orientation: Qt.Horizontal //  Панели стоят в ряд (слева направо)


    ImageContainer {

        id: sourceViewer

        //  Настройки размеров для root
        SplitView.preferredWidth: root.width / 2 //  половина ширины
        //  Минимальный порог сжатия
        SplitView.minimumWidth: 100

    }


    //  Правое окно: Результат обработки (нормализация и т.д.)
    ImageContainer {

        id: processedView

        //  Настройки размеров для root
        SplitView.preferredWidth: root.width / 2 //  половина ширины
        //  Минимальный порог сжатия
        SplitView.minimumWidth: 100

        backgroundColor: "black"
        labelText: "!!!!"

        handler.directionOut: false
    }

}
