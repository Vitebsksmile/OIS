import QtQuick          //  Базовые элементы (Rectangle, Image, Text)
import QtQuick.Controls //  Стандартные элементы (Window, Button, Popup, Menu, SplitView)


Rectangle {

    id: root
    implicitWidth: 200  //  Рекомендуемая ширина (важно для Layout)
    implicitHeight: 200 //  Рекомендуемая высота
    color: "lightblue"
    radius: 10


    //  Экспортируем внутренние объекты наружу,
    //  чтобы обращаться к ним как root.handler или root.popup
    property alias handler: fileHandler
    property alias popup: statusPopup
    //property alias imageSource: mainImage.source  //  Прямой доступ к источнику

    //  Создаем экземпляр вашего C++ класса прямо внутри QML
    FileHandler {id: fileHandler }

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
        Behavior on opacity { NumberAnimation { duration: 500 } }

        //  Отключаем кеш, чтобы увидеть изменения при перезагрузке того же файла
        //  Заставляем QML перечитывать файл с диска каждый раз (нужно для OpenCV)
        cache: false
    }

    //  Текст виден только если в Image ничего не загружено (Null)
    Text {
        visible: mainImage.status === Image.Null
        anchors.centerIn: parent    //  Центрируем надпись
        text: "фото не выбрано"
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
        modal: false    //  Не блокирует взаимодействие с основным окном
        focus: false    //  Не перехватывает ввод (Escape не сработает при false)

        //  Закроется, если нажать Esc (нужен focus: true) или кликнуть мимо
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

        Text {
            anchors.centerIn: parent    //  Центрируем надпись
            text: statusPopup.message   //  Отображаем текст из свойства выше
        }
    }
}
