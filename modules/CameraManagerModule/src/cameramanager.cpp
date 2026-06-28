#include "cameramanager.h"
#include <QDebug>


// --- РУЧНОЕ ОБЪЯВЛЕНИЕ СИГНАТУР ИЗ BASLER PYLON C-API ---
// Избавляет от конфликтов типов (wchar_t) между MSVC-заголовками Basler и компилятором MinGW64

typedef int GENAPIC_RESULT;              // В Pylon Си-интерфейсе все результаты — это int
#define GENAPI_E_OK 0                    // Статус успешного выполнения функции


// Константы режимов доступа, если понадобятся
#define PYLON_C_ACCESS_MODE_CONTROL 2
#define PYLON_C_ACCESS_MODE_STREAM  4


// Объявляем Си-функции из DLL Basler с соглашением о вызовах __cdecl (стандарт для x64 Windows)
extern "C" {
    GENAPIC_RESULT __cdecl pylonInitialize(void);
    GENAPIC_RESULT __cdecl pylonTerminate(void);
    // Добавим функции для открытия камеры, которые понадобятся дальше:
    GENAPIC_RESULT __cdecl pylonEnumerateDevices(size_t* numDevices);
    GENAPIC_RESULT __cdecl pylonCreateDeviceByIndex(size_t index, PYLON_DEVICE_HANDLE* phDev);
    GENAPIC_RESULT __cdecl pylonDeviceOpen(PYLON_DEVICE_HANDLE hDev, int accessMode);
    bool __cdecl pylonDeviceIsOpen(PYLON_DEVICE_HANDLE hDev);
    GENAPIC_RESULT __cdecl pylonDeviceClose(PYLON_DEVICE_HANDLE hDev);
    GENAPIC_RESULT __cdecl pylonDestroyDevice(PYLON_DEVICE_HANDLE hDev);
}


// --- РЕАЛИЗАЦИЯ КЛАССА CAMERAMANAGER ---

CameraManager::CameraManager(QObject *parent)
    : QObject(parent), isStreaming(false), hDev(nullptr)
{
    //  Инициализируем ресурсы Pylon Runtime
    GENAPIC_RESULT result = pylonInitialize();

    if (result != GENAPI_E_OK) {
        qCritical() << "Не удалось инициализировать Pylon C-API! Код ошибки:" << result;
    } else {
        qDebug() << "Pylon C_API успешно инициализирован.";
    }
}

CameraManager::~CameraManager()
{
    //  Безопасно закрываем камеру перед уничтожением объекта
    closeCamera();

    //  Освобождаем ресурсы Pylon Runtime
    pylonTerminate();
    qDebug() << "Ресурсы Pylon C-API освобождены.";
}

bool CameraManager::openFirstCamera()
{
    GENAPIC_RESULT result;
    size_t numDevices = 0;

    //  1. Опрашиваем систему нга наличие подключенных камер Basler
    result = pylonEnumerateDevices(&numDevices);
    if (result != GENAPI_E_OK || numDevices == 0) {
        qWarning() << "Камеры Basler не обнаружены или ошибка опроса. Код ошибки: " << result;
        return false;
    }

    qDebug() << "Найдено камер Basler в системе: " << numDevices;

    //  2. Создаем внутренний дескриптор для самой первой камеры (индекс 0)
    result = pylonCreateDeviceByIndex(0, &hDev);
    if (result != GENAPI_E_OK) {
        qCritical() << "Не удалось создать дескриптор устройства. Код ошибки: " << result;
        hDev = nullptr;
        return false;
    }

    //  3. Открываем физический доступ к камере (управление настройками + захват видеопотока)
    result = pylonDeviceOpen(hDev, PYLON_C_ACCESS_MODE_CONTROL | PYLON_C_ACCESS_MODE_STREAM);
    if (result != GENAPI_E_OK) {
        qCritical() << "Не удалось открыть камеру. Код ошибки: " << result;
        pylonDestroyDevice(hDev);   //  Удаляем дескриптор, если открыть не удалось
        hDev = nullptr;
        return false;
    }

    qDebug() << "Камера Basler успешно открыта и готова к работе!";
    return true;
}

void CameraManager::closeCamera()
{
    if (hDev != nullptr) {
        //  Проверяем, открыта ли камера в данный момент
        if (pylonDeviceIsOpen(hDev)) {
            pylonDeviceClose(hDev);
        }

        //  Уничтожаем дескриптор и освобождаем выделенную под него память драйвера
        pylonDestroyDevice(hDev);
        hDev = nullptr;
        qDebug() << "Камера закрыта, дескриптор успешно уничтожен.";
    }
}
