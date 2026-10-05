import QtQuick 2.15
import QtMultimedia

Rectangle {
    id: root
    property string frameSource: ""
    color: "#065F46"
    border.color: "#047857"
    border.width: 2

    VideoOutput {
        id: videoOutput
        anchors.fill: parent

        //  Fill setting: PreserveAspectFit or PreserveAspectCrop
        fillMode: VideoOutput.PreserveAspectFit
    }

    //  Текст виден только если в Image ничего не загружено (Null)
    NormalText {
        id: statusText
        visible: videoOutput.videoSink.videoSize.width === 0 || videoOutput.videoSink.videoSize.height === 0
        anchors.centerIn: parent //      Центрируем надпись
        //Layout.alignment: Qt.AlignHCenter
        text: qsTr("No signal")
        //color: "gray"
    }

    VideoProvider {
        id: videoProvider
        videoSink: videoOutput.videoSink

        Component.onCompleted: {
            videoProvider.setFrameSource(root.frameSource)
            //  Pass object to C++
            VideoStreamController.registerProvider(videoProvider)
        }
    }
}
