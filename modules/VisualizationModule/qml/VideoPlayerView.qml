import QtQuick
import QtMultimedia

Rectangle {
    id: root

    width: 800
    height: 600
    color: "black"

    MediaPlayer {
        id: player

        //source: "file:///E:/20260416_192406.mp4"
        source: "http://192.168.0.112:4747/video"
        //source: "http://192.168.0.112"
        //source: "http://10.220.18.94:4747/video"

        videoOutput: videoOutput

        onErrorOccurred: (error, errorString) => {
                             console.log("Ошибка видео: ", errorString)
                         }

        onMetaDataChanged: {
            console.log("--- Информация о потоке ---")
            // В Qt 6 метаданные доступны через ключи в объекте metaData
            console.log("Название:", metaData.value(MediaMetaData.Title))
            console.log("Формат/Контейнер:", metaData.value(MediaMetaData.FileFormat))
            console.log("Длительность:", (metaData.value(MediaMetaData.Duration) / 1000).toFixed(2), "сек")
        }

        onVideoTracksChanged: {
            if (player.videoTracks.length > 0) {
                // Берем первый видеопоток
                let track = player.videoTracks[0]

                // В некоторых версиях Qt 6 свойства доступны напрямую у объекта трека
                let size = track.videoFrameSize

                if (size) {
                    console.log("Разрешение кадра:", size.width, "x", size.height)
                } else {
                    console.log("Разрешение еще не определено...")
                }

                console.log("Частота кадров:", track.frameRate)
            }
        }
    }

    VideoOutput {
        id: videoOutput
        anchors.fill: parent
    }

    Component.onCompleted: {
        player.play()
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (player.playbackState === MediaPlayer.PlayingState) {
                player.pause()
            } else {
                player.play()
            }
        }
    }
}
