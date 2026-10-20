#include "systemenvironment.h"
#include <QNetworkInterface>
#include <QStandardPaths>

SystemEnvironment::SystemEnvironment(QObject *parent) {}

const QString SystemEnvironment::currentMacAddress() const
{
    //  Получение физического (MAC) адреса активной сетевой карты
    QString macAddress = "Not found";
    const auto interfaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface &interface : interfaces) {
        // Отсекаем петлевой интерфейс (localhost) и неактивные карты
        if (interface.flags().testFlag(QNetworkInterface::IsLoopBack) ||
            !interface.flags().testFlag(QNetworkInterface::IsUp)) {
            continue;
        }

        QString hardwareAddress = interface.hardwareAddress();
        if (!hardwareAddress.isEmpty()) {
            macAddress = hardwareAddress;
            break; // Берем первый попавшийся активный физический адрес
        }
    }
    return macAddress;
}

QDir SystemEnvironment::dir()
{
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    return dir;
}
