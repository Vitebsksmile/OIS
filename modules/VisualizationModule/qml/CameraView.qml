import QtQuick
import QtMultimedia

Rectangle {

    id: root

    color: "lightgreen" //      Цвет по умолчанию
    radius: 10


    //  Объект для доступа к списку устройств
    MediaDevices {
        id: devices
    }


    //  Компонент камеры
    Camera {
        id: camera

        //  Указываем конкретную камеру
        cameraDevice: {
            for (var i = 0; i < devices.videoInputs.length; i++) {
                if (devices.videoInputs[i].description.includes("DroidCam")) {
                    return devices.videoInputs[i]
                }
            }
            //  Если DroidCam не найден - берем камеру по умолчанию
            return devices.defaultVideoInput
        }
    }


    //  Элемент для отображения видео
    VideoOutput {
        id: videoOutput
        anchors.fill: parent

        //  Заполнение с сохранением пропорций
        fillMode: VideoOutput.PreserveAspectCrop
    }


    //  Сессия захвата, связывающая камеру и вывод
    CaptureSession {
        camera: camera
        videoOutput: videoOutput
    }


    Component.onCompleted: {
        //  Выводим в консоль все найденные камеры для отладки
        for (var i = 0; i < devices.videoInputs.length; i++) {
            console.log("CameraView: Найдена камера: ", devices.videoInputs[i].description)
        }

        camera.start()  //  Запуск камеры при загрузке
    }

}
