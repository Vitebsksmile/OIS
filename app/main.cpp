#include <QCoreApplication>
#include <QDebug>

#include "application.h"


int main(int argc, char *argv[])
{

    // Устанавливаем атрибуты приложения
    //  Устанавливаем официальное имя приложения для системы
    QCoreApplication::setApplicationName(APP_NAME_STR);
    QCoreApplication::setOrganizationName("IS-23");

    // Создаем и инициализируем приложение
    Application app;


    if (!app.initialize()) {
        qCritical() << "Failed to initialize application";
        return -1;
    }

    // Запускаем приложение
    return app.run(argc, argv);

}
