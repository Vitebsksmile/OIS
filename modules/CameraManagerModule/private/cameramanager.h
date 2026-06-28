#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include <QObject>


//  Объявляем дескриптор камеры как указатель
typedef void* PYLON_DEVICE_HANDLE;


class CameraManager : public QObject
{
    Q_OBJECT
public:
    explicit CameraManager(QObject *parent = nullptr);
    ~CameraManager();

    bool openFirstCamera();     //  Метод для поиска и открытия первой камеры
    void closeCamera();         //  Метод для закрытия и освобождения камеры

private:
    bool isStreaming;
    PYLON_DEVICE_HANDLE hDev;   //  Дескриптор открытого устройства камеры
};

#endif // CAMERAMANAGER_H
