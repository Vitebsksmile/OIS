import QtQuick
import QtQuick.Controls

Rectangle {
    id: root

    //  Целевые размеры и координаты, куда окно должно прийти в итоге анимации
    property int targetX: 100       //  Целевая позиция по горизонтали
    property int targetY: 100       //  Целевая позиция по вертикали
    property int targetWidth: 400   //  Ширина окна в открытом состоянии
    property int targetHeight: 280  //  Высота окна в открытом состоянии


    //  Визуальный стиль и начальное состояние
    color: "white"
    radius: 15
    clip: true              //  Обрезать всё содержимое, выходящее за границы радиуса
    opacity: 0              //  Начальная прозрачность (невидимо)
    visible: opacity > 0    //  Автоматическое скрытие из отрисовки, если прозрачность 0 (оптимизация)
    z: 10                   //  Всегда поверх размытия


    //  Функции (Логика управления)
    function open(startX, startY, startW, startH) {
        //  Сначала прыгаем в координаты иконки без анимации
        state = ""      //  Сброс текущего состояния (прерывает активные анимации)
        x = startX      //  Мгновенно перемещаем в X-координату иконки
        y = startY      //  Мгновенно перемещаем в Y-координату иконки
        width = startW  //  Устанавливаем ширину как у иконки
        height = startH //  Устанавливаем высоту как у иконки

        //  Переключаем в состояние "show", запуская анимацию
        state = "show"
    }

    function close() {
        state = ""// Возврат к базовому состоянию (всё станет прозрачным и вернется в начало)
    }


    //  Состояния (States)
    states: [
        State { name: "hidden" },   //  Пустое состояние (все свойства берутся из начальных значений выше)
        State {
            name: "show"            //  Состояние "открыто"
            PropertyChanges {       //  Список свойств, которые должны измениться в этом состоянии:
                root.opacity: 1
                root.x: root.targetX
                root.y: root.targetY
                root.width: root.targetWidth
                root.height: root.targetHeight
                root.radius: 0 // Убираем скругление при раскрытии
            }
        }
    ]


    //  Анимация (Transitions)
    transitions: Transition {                       //  Описание того, КАК переходить между состояниями
        // Анимируем всё сразу для эффекта "вырастания"
        NumberAnimation {                           //  Анимация числовых значений
            properties: "x,y,width,height,opacity"  //  Перечисляем, что анимируем
            duration: 500                           //  Длительность в миллисекундах (0.5 сек)
            easing.type: Easing.OutQuint            //  Тип сглаживания: быстрое начало, очень плавный конец
        }
    }

    //  Содержимое виджета
    Text {
        anchors.centerIn: parent// Текст всегда по центру окна
        text: "Контент виджета"

        // Плавное появление текста: виден только когда окно расширилось больше 100px
        opacity: root.width > 100 ? 1 : 0
    }

    //  Закрытие при клике по самому виджету
    MouseArea {
        anchors.fill: parent

        // 1. Указываем, что события НЕ должны идти дальше к элементам под нами
        propagateComposedEvents: false
    }

    // Кнопка закрытия внутри виджета
    Button {
        anchors.right: parent.right; anchors.top: parent.top    //  Кнопка в верхнем правом углу
        text: "—"; flat: true                                   //  Плоская кнопка без рамки
        onClicked: root.state = "hidden"                        //  При клике переходим в скрытое состояние
    }
}
