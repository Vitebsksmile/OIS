#ifndef BASLERDRIVER_H
#define BASLERDRIVER_H

#include "ICameraDriver.h"
#include "cameraconfig.h"


//  Объявляем дескриптор камеры как указатель
typedef void* PYLON_DEVICE_HANDLE;


class BaslerDriver final: public ICameraDriver
{
public:

    BaslerDriver();
    ~BaslerDriver() override;

    //  Метод для поиска и открытия первой камеры
    bool connect(const CameraConfig& config) override;

    //  Метод для закрытия и освобождения камеры
    void disconnect() override;

private:
    PYLON_DEVICE_HANDLE hDev;   //  Дескриптор открытого устройства камеры
};

#endif // BASLERDRIVER_H
