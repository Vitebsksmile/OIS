//  Базовые элементы (Rectangle, Image, Text)
import QtQuick

//  Стандартные элементы (Window, Button, Popup, Menu, SplitView)
import QtQuick.Controls

//import VisualizationModule 1.0

Rectangle {

    id: root

    //  Экспортируем внутренние объекты наружу,
    //  чтобы обращаться к ним как root.handler или root.popup
    property alias handler: fileHandler //      Текущий путь к изображению
    property alias popup: statusPopup //      Всплывающее окно
    property alias backgroundColor: root.color //      Задать цвет снаружи
    property alias labelText: statusText.text //      Задать текст статуса загрузки изображения снаружи
    //property alias imageSource: root.source   //      Прямой доступ к источнику


    //  Создаем экземпляр 'Файлового обработчика' C++ класса 'FileHandler'
    FileHandler {
        id: fileHandler
    }

    implicitWidth: 200 //  Рекомендуемая ширина (важно для Layout)
    implicitHeight: 200 //  Рекомендуемая высота
    color: "lightblue" //      Цвет по умолчанию
    radius: 10

    Image {

        id: mainImage
        anchors.margins: 5

        //  Заполняем изображением всю площадь родителя
        anchors.fill: parent

        //  Сохраняем пропорции изображения
        fillMode: Image.PreserveAspectFit

        //  Автоматическая связь с c++ свойством
        //  Автоматически обновляем картинку, когда в C++ срабатывает сигнал
        source: fileHandler.currentImagePath

        //  Анимация: если прозрачность (opacity) изменится, это произойдет плавно за 0.5 сек
        Behavior on opacity {
            NumberAnimation {
                duration: 500
            }
        }

        //  Отключаем кеш, чтобы увидеть изменения при перезагрузке того же файла
        //  Заставляем QML перечитывать файл с диска каждый раз (нужно для OpenCV)
        cache: false
    }

    //  Текст виден только если в Image ничего не загружено (Null)
    Text {

        id: statusText

        visible: mainImage.status === Image.Null
        anchors.centerIn: parent //      Центрируем надпись
        text: qsTr("No image selected") //      Значение по умолчанию
        color: "gray"
    }

    //  Всплывающее окно для коротких сообщений (аналог Toast в Android)
    Popup {

        id: statusPopup

        property string message: "" //  Свое свойство для текста ошибки или успеха

        //  Центрируем по горизонтали и поднимаем на 100 пикселей от низа
        x: (parent.width - width) / 2
        y: parent.height - 100

        width: 200
        height: 40

        //  Настройки закрытия всплывающего окна
        modal: false //  Не блокирует взаимодействие с основным окном
        focus: false //  Не перехватывает ввод (Escape не сработает при false)

        //  Закроется, если нажать Esc (нужен focus: true) или кликнуть мимо
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

        Text {

            anchors.centerIn: parent //  Центрируем надпись
            text: statusPopup.message //  Отображаем текст из свойства выше
        }
    }
}
