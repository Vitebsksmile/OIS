import QtQuick

Item {

    property alias source: image.source

    Image {
        id: image
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
    }

    DetectionOverlay {
        anchors.fill: image
    }
}
