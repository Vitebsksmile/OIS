import QtQuick
import QtQuick.Layouts

Rectangle {

    radius: 10

    RowLayout {

        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 10

        IconButton {
            //buttonName: "Take a photo"
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            iconWidth: 100
            iconHeight: 50
        }

        IconButton {
            iconSource: "../qrc/setCamcoder.svg"
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            iconWidth: 100
            iconHeight: 70
        }

    }

}
