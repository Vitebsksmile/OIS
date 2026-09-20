import QtQuick
import QtMultimedia

Rectangle {
    id: root

    color: "#bf2608"
    border.color: "#eb3a17"
    border.width: 2

    VideoOutput {
        id: videoOutput
        anchors.fill: parent

        //  Fill setting: PreserveAspectFit or PreserveAspectCrop
        fillMode: VideoOutput.PreserveAspectFit
    }

    VideoProvider {
        id: videoProvider
        videoSink: videoOutput.videoSink

        Component.onCompleted: {
            videoProvider.setFrameSource("camera")
            //  Pass object to C++
            VideoStreamController.registerProvider(videoProvider)
        }
    }
}
