#include <QGuiApplication>
#include <QQmlApplicationEngine>
//#include <QQmlContext>  //  нужен для передачи модели в QML

#include "MyModel.h"


int main(int argc, char *argv[])
{
    // QGuiApplication — база для графических приложений (без виджетов)
    QGuiApplication app(argc, argv);

    /*MyModel model;  //  создаем объект данных

    QTimer::singleShot(3000, [&model]() {
        model.addItem("New model (for timer)");
    });*/

    QQmlApplicationEngine engine;

    //  ГЛАВНОЕ: Регестрируем нашу роль в контексте QML под именем 'myGlobalModel'
    /*engine.rootContext()->setContextProperty("myGlobalModel", &model);*/

    // Загружаем файл. Путь формируется автоматически через CMake (см. ниже)
    const QUrl url("qrc:/qt/qml/path/main.qml");

    QObject::connect(&engine,
                     &QQmlApplicationEngine::objectCreationFailed,
                     &app,
                     []() { QCoreApplication::exit(-1); },
                     Qt::QueuedConnection);

    engine.load(url);  //   загружаем интерфейс


    return app.exec();
}
