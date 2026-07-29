#include <QDebug>

#include "baslerdriver.h"
#include "cameraconfig.h"


// --- РУЧНОЕ ОБЪЯВЛЕНИЕ СИГНАТУР ИЗ BASLER PYLON C-API ---
// Избавляет от конфликтов типов (wchar_t) между MSVC-заголовками Basler и компилятором MinGW64
typedef int GENAPIC_RESULT;              // В Pylon Си-интерфейсе все результаты — это int
#define GENAPI_E_OK 0                    // Статус успешного выполнения функции


// Константы режимов доступа, если понадобятся
#define PYLON_C_ACCESS_MODE_CONTROL 2
#define PYLON_C_ACCESS_MODE_STREAM  4


// Объявляем Си-функции из DLL Basler с соглашением о вызовах __cdecl (стандарт для x64 Windows)
extern "C" {
    GENAPIC_RESULT __cdecl PylonInitialize(void);
    GENAPIC_RESULT __cdecl PylonTerminate(void);
    // Добавим функции для открытия камеры, которые понадобятся дальше:
    GENAPIC_RESULT __cdecl PylonEnumerateDevices(size_t* numDevices);
    GENAPIC_RESULT __cdecl PylonCreateDeviceByIndex(size_t index, PYLON_DEVICE_HANDLE* phDev);
    GENAPIC_RESULT __cdecl PylonDeviceOpen(PYLON_DEVICE_HANDLE hDev, int accessMode);

    //int __cdecl PylonDeviceIsOpen(PYLON_DEVICE_HANDLE hDev);
    bool __cdecl PylonDeviceIsOpen(PYLON_DEVICE_HANDLE hDev);

    GENAPIC_RESULT __cdecl PylonDeviceClose(PYLON_DEVICE_HANDLE hDev);
    GENAPIC_RESULT __cdecl PylonDestroyDevice(PYLON_DEVICE_HANDLE hDev);
}


// --- РЕАЛИЗАЦИЯ КЛАССА ICameraDriver ---

/*BaslerDriver::BaslerDriver(QObject *parent)
    : ICameraDriver(parent), hDev(nullptr)
{
    //  Инициализируем глобальные ресурсы Pylon Runtime один раз при создании драйвера
    GENAPIC_RESULT result = PylonInitialize();

    if (result != GENAPI_E_OK) {
        qCritical() << "Не удалось инициализировать Pylon C-API! Код ошибки:" << result;
    } else {
        qDebug() << "Pylon C_API успешно инициализирован.";
    }
}
*/

BaslerDriver::BaslerDriver()
    : hDev(nullptr)
{
    //  Инициализируем глобальные ресурсы Pylon Runtime один раз при создании драйвера
    GENAPIC_RESULT result = PylonInitialize();

    if (result != GENAPI_E_OK)
    {
        qCritical() << "BaslerDriver: Не удалось инициализировать Pylon C-API! Код ошибки:" << result;
    } else {
        qDebug() << "BaslerDriver: Pylon C_API initialization was successful.";
    }
}

BaslerDriver::~BaslerDriver()
{
    //  Безопасно закрываем камеру и освобождаем дескриптор, если это не было сделано вручную
    BaslerDriver::disconnect();

    //  Освобождаем глобальные ресурсы Pylon Runtime
    PylonTerminate();
    qDebug() << "BaslerDriver: Ресурсы Pylon C-API освобождены.";
}


bool BaslerDriver::connect(const CameraConfig& config)
{
    if (hDev != nullptr) {
        qWarning() << "BaslerDriver: Камера уже подключена.";
        return false;
    }

    GENAPIC_RESULT result;
    size_t numDevices = 0;

    //  1. Опрашиваем систему на наличие подключенных камер
    result = PylonEnumerateDevices(&numDevices);
    if (result != GENAPI_E_OK || numDevices == 0) {
        qWarning() << "BaslerDriver: Камеры Basler не обнаружены или ошибка опроса. Код ошибки: " << result;
        return false;
    }
    qDebug() << "BaslerDriver: Найдено камер Basler в системе: " << numDevices;

    //  2. Создаем внутренний дескриптор устройства
    //  В будущем здесь можно использовать pylonGetDeviceInfo и фильтровать по config.serialNumber.
    //  Пока для простоты создаем по индексу 0, т.к. первая виртуальная камера в pylon SDK имеет индекс '0'
    result = PylonCreateDeviceByIndex(0, &hDev);
    if (result != GENAPI_E_OK) {
        qCritical() << "BaslerDriver: Не удалось создать дескриптор устройства. Код ошибки: " << result;
        hDev = nullptr;
        return false;
    }

    //  3. Открываем физический доступ к камере (управление настройками + захват видеопотока)
    result = PylonDeviceOpen(hDev, PYLON_C_ACCESS_MODE_CONTROL | PYLON_C_ACCESS_MODE_STREAM);
    if (result != GENAPI_E_OK) {
        qCritical() << "BaslerDriver: Не удалось открыть камеру. Код ошибки: " << result;
        PylonDestroyDevice(hDev);   //  Удаляем дескриптор, если открыть не удалось
        hDev = nullptr;
        return false;
    }

    //  4. ТАКЖЕ: Здесь, используя объект config, можно применить параметры в камеру
    //  ()например, установить config.exposure или config.gain через GenApi C-APY)

    qDebug() << "BaslerDriver: Камера Basler успешно открыта и готова к работе!";
    return true;
}


void BaslerDriver::disconnect()
{
    if (hDev != nullptr) {
        //  Проверяем, открыта ли камера в данный момент
        if (PylonDeviceIsOpen(hDev)) {
            PylonDeviceClose(hDev);
        }

        //  Уничтожаем дескриптор и освобождаем выделенную под него память драйвера
        PylonDestroyDevice(hDev);
        hDev = nullptr;
        qDebug() << "BaslerDriver: Камера закрыта, дескриптор успешно уничтожен.";
    }
}
