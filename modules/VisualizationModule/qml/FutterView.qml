import QtQuick
import QtQuick.Controls
//import QtMultimedia


SplitView {

    id: otherContainer

    orientation: Qt.Horizontal  //  Панели стоят в ряд (слева направо)

    CameraView {

        //color: Qt.rgba(255/255, 71/255, 77/255, 0.9) //  Light Red
        //radius: 10

        //  Настройки размеров для otherContainer
        SplitView.preferredWidth: 200   //  Желаемая ширина при старте
        SplitView.minimumWidth: 100     //  Минимальный порог сжатия
        SplitView.maximumWidth: 500     //  Максимальный порог растяжения

    }

    VideoPlayerView {

        //color: Qt.rgba(255/255, 71/255, 77/255, 0.9) //  Light Red
        //radius: 10

        //  Настройки размеров для otherContainer
        SplitView.preferredWidth: 200   //  Желаемая ширина при старте
        SplitView.minimumWidth: 100     //  Минимальный порог сжатия
        SplitView.maximumWidth: 500     //  Максимальный порог растяжения

    }


    Rectangle {

        id: root

        //  Настройки размеров для otherContainer
        SplitView.preferredWidth: 200   //  Желаемая ширина при старте
        SplitView.minimumWidth: 100     //  Минимальный порог сжатия
        SplitView.maximumWidth: 500     //  Максимальный порог растяжения

        color: "lightgreen" //      Цвет по умолчанию
        radius: 10

    }


    Rectangle {

        color: Qt.rgba(255/255, 71/255, 77/255, 0.9) //  Light Red
        radius: 10

        //  Настройки размеров для otherContainer
        SplitView.fillWidth: true       //  Занимает всё оставшееся место справа
        SplitView.minimumWidth: 100     //  Минимальный порог сжатия

    }

}
