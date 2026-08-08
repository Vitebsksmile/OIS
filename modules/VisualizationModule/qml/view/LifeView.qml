import QtQuick 2.15
import QtMultimedia
import VisualizationModule

Rectangle {
    id: root

    //anchors.fill: parent
    color: "#065F46"
    border.color: "#047857"
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
            //  Pass object to C++
            FileHandlerManager.registerProvider(videoProvider)
        }
    }
}
