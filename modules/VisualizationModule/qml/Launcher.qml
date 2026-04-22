import QtQuick
import QtQuick.Window

Item {
    //  Окно выбора (Лаунчер)
    Window {
        id: launcherWindow

        visible: true
        width: 400
        height: 300

        //  Убираем заголовок и рамки
        flags: Qt.FramelessWindowHint | Qt.Window
        color: "#2c3e50"
        opacity: 1.0

        MouseArea {
            anchors.fill: parent
            onPressed: (mouse) => { launcherWindow.startSystemMove() }
        }

        Item {
            id: rootContainer
            anchors.fill: parent
            scale: 1.0

            //  Групповая анимация "провала"
            ParallelAnimation {
                id: collapseAnimation

                //  Уменьшение масштаба
                NumberAnimation {
                    target: rootContainer
                    property: "scale"
                    from: 1.0
                    to: 0.8
                    duration: 400
                    easing.type: Easing.InBack  //  Эффект небольшого отскока назад
                }

                //  Исчизновение
                NumberAnimation {
                    target: launcherWindow
                    property: "opacity"
                    from: 1.0
                    to: 0.0
                    duration: 350
                }

                //  Когда все анимации закончились:
                onFinished: {
                    launcherWindow.close()  //  Закрываем Лаунчер
                    mainWindow.show()       //  Открываем главное окно в выбранном режиме
                }
            }


            Row {
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.margins: 30
                spacing: 40

                ModeButton {

                    id: setCamera

                    width: 260
                    height: 260

                    // Переопределяем картинку через созданный алиас
                    iconSource: "qrc/setCamera.svg"

                    onRequestOpen: (globalX, globalY, globalW, globalH) => {

                                       collapseAnimation.start() //  Запускаем анимацию Лаунчера
                                       riseAnimation.start()  //  Запускаем анимацию Главного окна

                                       console.log("Launcher: Camera mode selected.")
                                   }
                }

                ModeButton {

                    id: setCamcoder

                    width: 260
                    height: 260

                    // Переопределяем картинку через созданный алиас
                    iconSource: "qrc/setCamcoder.svg"

                    onRequestOpen: (globalX, globalY, globalW, globalH) => {

                                       collapseAnimation.start() //  Запускаем анимацию Лаунчера
                                       riseAnimation.start()  //  Запускаем анимацию Главного окна

                                       console.log("Launcher: Camcoder mode selected.")
                                   }
                }
            }
        }


    }

    Window {
        id: mainWindow
        visible: false  //  Скрыто по умолчанию
        width: 1280
        height: 720
        title: "Main Application"

        Item {
            id: mainContainer
            anchors.fill: parent
            scale: 1.2

            //  Анимация появления
            onVisibleChanged: {
                if (visible) riseAnimation.start()
            }

            ParallelAnimation {
                id: riseAnimation

                //  "Выплывания" навстречу
                NumberAnimation {
                    target: mainContainer
                    property: "scale"
                    from: 1.2
                    to: 1.0
                    duration: 400
                    easing.type: Easing.InBack  //  Эффект "выплывания" навстречу
                }

                //  Появление
                NumberAnimation {
                    target: mainWindow
                    property: "opacity"
                    to: 1.0
                    duration: 500
                }
            }

            //  Здесь основной контент
        }
    }
}
