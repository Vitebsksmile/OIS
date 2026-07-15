#ifndef BASLERDRIVER_H
#define BASLERDRIVER_H

#include <QObject>

#include "ICameraDriver.h"
#include "cameraconfig.h"


//  Объявляем дескриптор камеры как указатель
typedef void* PYLON_DEVICE_HANDLE;


class BaslerDriver : public ICameraDriver
{
    Q_OBJECT
public:
    explicit BaslerDriver(QObject *parent = nullptr);
    ~BaslerDriver();

    bool connect(const CameraConfig& config);     //  Метод для поиска и открытия первой камеры
    void disconnect();         //  Метод для закрытия и освобождения камеры

private:
    PYLON_DEVICE_HANDLE hDev;   //  Дескриптор открытого устройства камеры
};

#endif // BASLERDRIVER_H
