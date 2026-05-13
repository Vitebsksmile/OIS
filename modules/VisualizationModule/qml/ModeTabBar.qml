import QtQuick
import QtQuick.Layouts

Rectangle {

    radius: 10

    RowLayout {

        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 10

        IconButton {
            iconSource: "../qrc/setCamera6.svg"
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            iconWidth: 100
            iconHeight: 100
            iconRadius: 50
            imgMargins: 5
        }

        IconButton {
            iconSource: "../qrc/setVideocamera11.svg"
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            iconWidth: 100
            iconHeight: 100
            iconRadius: 50
            imgMargins: 14
        }

    }

}
