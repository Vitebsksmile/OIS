import QtQuick

Rectangle {
    id: buttonRoot

    //  Свойство для удобной смены пути к картинке извне
    property alias iconSource: img.source


    // Сигнал, который отдаёт координаты и размеры кнопки для 'раскрытия' виджета из этой кнопки
    signal requestOpen(real x, real y, real weight, real height)


    width: 160
    height: 160
    radius: 30

    // Меняем цвет в зависимости от наведения (hover)
    color: mouseArea.containsMouse ? "#5dade2" : "#3498db"

    // Плавный переход цвета
    Behavior on color {
        ColorAnimation { duration: 150 }
    }


    // Сама картинка
    Image {

        id: img
        //source: "qrc/setCamera.svg"         //  Путь к файлу
        anchors.fill: parent
        anchors.margins: 10                 //  Отступ, чтобы картинка не касалась краев
        fillMode: Image.PreserveAspectFit   //  Сохранять пропорции
        antialiasing: true                  //  Сглаживание краев графического элемента

        // Опционально: картинка чуть увеличиваться при наведении
        scale: mouseArea.containsMouse ? 1.1 : 1.0
        Behavior on scale { NumberAnimation { duration: 150 } }

    }


    MouseArea {

        id: mouseArea
        anchors.fill: parent

        // Позволяет containsMouse работать (для анимации иконки)
        hoverEnabled: true


        onClicked: {
            // Мапим центральную точку иконки на всё окно (окно родителя)
            var centerCoords = buttonRoot.mapToItem(null, 0, 0)

            //  Испускаем сигнал, передавая координаты центра
            buttonRoot.requestOpen(centerCoords.x, centerCoords.y, buttonRoot.width, buttonRoot.height)

        }

    }

}
