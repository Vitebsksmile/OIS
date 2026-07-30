#include "cameramanagerservice.h"

#include <QDebug>

#include "cameraconfig.h"
#include "ICameraDriver.h"
#include "baslerdriver.h"

CameraManagerService::CameraManagerService(QObject* parent)
    : ICameraManagerService(parent)
{
    qDebug() << "CameraManagerService: creat new object";
    m_cameraDriver.reset(new BaslerDriver());
    qDebug() << "after new";
}

CameraManagerService::~CameraManagerService() = default;

void CameraManagerService::onStartReady()
{
    qDebug() << "CameraManagerService: Получен сигнал готовности к старту. Инициируем проверку камер...";

    // Вызываем ваш метод проверки и подключения камеры
    checkAndConnectCamera();
}

void CameraManagerService::checkAndConnectCamera()
{
    qDebug() << "CameraManagerService: Запуск сканирования камер";

    CameraConfig config;

    bool isConnected = m_cameraDriver->connect(config);

    if (isConnected)
    {
        qDebug() << "CameraManagerService: Тестовое подключение прошло успешно.";
        m_cameraDriver->disconnect();   //  Освобождаем камеру после теста
    } else {
        qWarning() << "CameraManagerService: Камера не обнаружена или произошда ошибка инициализации.";
    }
}
