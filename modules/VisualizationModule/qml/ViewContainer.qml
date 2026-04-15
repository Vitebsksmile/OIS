import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {

    RowLayout {
        id: viewContainer
        Layout.fillWidth: true
        Layout.fillHeight: true

        //  Модель данных (изначально одно окно)
        ListModel {
            id: viewsModel
            ListElement { viewId: 1 }
        }

        //  Автоматический создатель окон
        Repeater {

            model: viewsModel

            delegate: Rectangle {

                //  Каждое окно втоматически растянется внутри RowLayout
                Layout.fillWidth: true
                Layout.fillHeight: true

                color: "red"

                //  Для передачи уникальных данных в каждое окно
                //source: "image://opencv/view_" + index
            }
        }
    }

    Button {
        text: "Add window"
        Layout.alignment: Qt.AlignHCenter

        onClicked: {
            //  Просто добавляем элемент в модель - QML сам создаст ImageView
            viewsModel.append({ "viewId": viewsModel.count })
        }
    }

}
