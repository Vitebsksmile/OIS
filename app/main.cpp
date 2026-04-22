#include <QCoreApplication>
#include <QDebug>
#include <QQuickStyle>
#include <QSysInfo>

#include "application.h"


int main(int argc, char *argv[])
{

    // Устанавливаем атрибуты приложения
    QCoreApplication::setOrganizationName(APP_ORG_NAME_STR);            // имя компании
    QCoreApplication::setOrganizationDomain(APP_DOMAIN_ORG_NAME_STR);   // домен компании
    QCoreApplication::setApplicationName(APP_NAME_STR);                 // имя приложения
    QCoreApplication::setApplicationVersion(APP_VERSION);               // версия приложения


    // 1. Динамический выбор стиля в зависимости от ОС
    #if defined(Q_OS_WIN)
        QQuickStyle::setStyle("Windows"); // Или "Windows", но Fusion лучше поддается кастомизации
    #elif defined(Q_OS_MACOS)
        QQuickStyle::setStyle("Basic");  // На macOS лучше использовать Basic для кастомных UI
    #elif defined(Q_OS_LINUX)
        QQuickStyle::setStyle("Material");
    #else
        QQuickStyle::setStyle("Basic");
    #endif


    // Создаем и инициализируем приложение
    Application app(argc, argv);


    if (!app.initialize()) {
        qCritical() << "Failed to initialize application";
        return -1;
    }


    // Запускаем приложение
    return app.run();

}
