import QtQuick
import QtQuick.Controls //  Стандартные элементы (Window, Button, Popup, Menu, SplitView)


//  Правая часть окна (центральный контент)
SplitView {

    id: root

    //  Экспортируем внутренние объекты наружу,
    //  чтобы обращаться к ним как <id>.handler или <id>.popup
    property alias handler: sourceViewer.handler   //  Текущий путь к изображению
    property alias popup: sourceViewer.popup       //  Всплывающее уведомление

    orientation: Qt.Vertical    //  Панели стоят колонной (сверху вниз)


    //  Вложенный разделитель для двух окон просмотра изображений
    //  (исходного изображения и предобработанного)
    SplitView {

        id: viewContainer

        orientation: Qt.Horizontal  //  Панели стоят в ряд (слева направо)

        //  Настройки размеров для root
        SplitView.fillHeight: true  //  Занимает всё оставшееся место сверху


        //  Левое окно: Исходное изображение
        //  (экземпляр компонента, определенного в ImageView.qml)
        ImageView {

            id: sourceViewer

            //  Настройки размеров для viewContainer
            SplitView.preferredWidth: viewContainer.width / 2  //  половина ширины
            //  Минимальный порог сжатия
            SplitView.minimumWidth: 100

            handler.directionOut: true

        }

        //  Правое окно: Результат обработки (нормализация и т.д.)
        ImageView {

            id: processedView

            //  Настройки размеров для viewContainer
            SplitView.preferredWidth: viewContainer.width / 2  //  половина ширины
            //  Минимальный порог сжатия
            SplitView.minimumWidth: 100

            backgroundColor: "black"
            labelText: "!!!!"

            handler.directionOut: false

        }

    }

}
