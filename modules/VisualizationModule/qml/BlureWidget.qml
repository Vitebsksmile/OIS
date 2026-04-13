import QtQuick
import QtQuick.Controls
import QtQuick.Effects

ApplicationWindow {

    Item {
        //  Контент, к-рый будем блюрить
        Item {
            id: backgroundLayer
            anchors.fill: parent

            Rectangle {
                anchors.fill: parent
                gradient: Gradient {
                    GradientStop {
                        position: 0
                        color: "#2c3e50"
                    }
                    GradientStop {
                        position: 1
                        color: "#000000"
                    }
                }
            }

            Row {
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.margins: 30
                spacing: 40

                ModeIconButton {

                    id: setCamera

                    width: 260
                    height: 260

                    iconSource: "qrc/setCamera.svg"

                    onRequestOpen: (globalX, globalY, globalW, globalH) => {

                                       // Переопределяем картинку через созданный вами алиас
                                       castomWidget.open(globalX, globalY,
                                                         globalW, globalH)
                                       console.log(
                                           "SelectionProcessingMode: Клик по иконке  ${setCamera} в координатах:",
                                           globalX, globalY)
                                   }
                }

                ModeIconButton {

                    id: setCamcoder

                    width: 260
                    height: 260

                    iconSource: "qrc/setCamcoder.svg"

                    onRequestOpen: (globalX, globalY, globalW, globalH) => {

                                       // Переопределяем картинку через созданный вами алиас
                                       castomWidget.open(globalX, globalY,
                                                         globalW, globalH)
                                       console.log(
                                           "SelectionProcessingMode: Клик по иконке  ${setCamcoder} в координатах:",
                                           globalX, globalY)
                                   }
                }
            }
        }

        //  Эффект размытия
        MultiEffect {
            anchors.fill: backgroundLayer
            source: backgroundLayer

            // Связываем радиус размытия с прозрачностью виджета для плавности
            blurEnabled: true
            blur: castomWidget.opacity //  Блюр привязан к прозрачности виджета
        }

        //  Виджет
        CustomWidget {
            id: castomWidget
            //  можно настроить финальную позицию для этого окна
            targetX: (parent.width - 400) / 2
            targetY: (parent.height - 280) / 2
            //targetX: 0
            //targetY: 0
            //targetWidth: window.width
            //targetHeight: window.height
        }
    }
}
