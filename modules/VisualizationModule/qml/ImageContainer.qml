import QtQuick
import QtQuick.Controls


SplitView {

    id: root

    //  Экспортируем внутренние объекты наружу,
    //  чтобы обращаться к ним как <id>.handler или <id>.popup
    property alias handler: imageView.handler //  Текущий экземпляр FileHandler
    property alias popup: imageView.popup //  Всплывающее уведомление
    property alias backgroundColor: imageView.backgroundColor //      Задать цвет снаружи
    property alias labelText: imageView.labelText //      Задать текст статуса загрузки изображения снаружи

    orientation: Qt.Vertical    //  Панели стоят колонной (сверху вниз)


    InformationView {
        id: informationView

        //  Настройки размеров для root
        SplitView.preferredHeight: informationView.implicitHeight   //  Желаемая высота при старте
        SplitView.minimumHeight: 10     //  Минимальный порог сжатия
        SplitView.maximumHeight: 150     //  Максимальный порог растяжения

        //  Прокидываем в InformationView.qml ссылки (alias) из ImageView.qml,
        //  используя экземпляр его родительского объекта с id: sourceViewer
        targetHandler: imageView.handler

    }


    ImageView {

        id: imageView

        //  Настройки размеров для root
        SplitView.fillHeight: true  //  Занимает всё оставшееся место сверху

        //  Минимальный порог сжатия
        SplitView.minimumWidth: 100

        handler.directionOut: true

    }

}
