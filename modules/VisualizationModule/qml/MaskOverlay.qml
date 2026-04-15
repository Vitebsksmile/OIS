/*
    Чтобы это заработало, убедитесь, что в target_link_libraries вашего модуля подключены:
    -   Qt6::Quick
    -   Qt6::Gui (для обработки изображений)

    Совет по интеграции с OpenCV:
    Чтобы не гонять тяжелые данные через QML-движок слишком часто, лучше всего:
    -   Выполнять поиск в отдельном потоке (Worker Thread) на C++.
    -   Передавать в QML только финальные координаты нормализованными (от 0.0 до 1.0).
    Это позволит маскам автоматически подстраиваться под любой размер ImageView на экране.
*/

import QtQuick
import QtQuick.Shapes


Item {

    id: root
    property var contours: []   //  Сюда придут данные из C++


    Repeater {

        model: root.contours    //  Для каждого найденного объекта создаем фигуру


        delegate: Shape {

            id: contourShape


            ShapePath {

                strokeColor: "lime" //  Цвет контура
                strokeWidth: 2
                fillColor: Qt.rgba(0, 1, 0, 0.2)    //  Полупрозрачная заливка объекта


                //  Создаем линии на основе точек из модели
                PathPolyline {

                    path: modelData.points  //  Массив точек [{x: 10, y: 20}, ...]

                }

            }
        }

    }

}
