#include <QGuiApplication>
#include <QQmlApplicationEngine>
//#include <QQmlContext>  //  нужен для передачи модели в QML


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;


    // Загружаем файл. Путь формируется автоматически через CMake (см. ниже)
    const QUrl url("qrc:/qt/qml/modules.VisualizationModule/Visualization.qml");

    //  логируем ошибки создания объектов qml
    QObject::connect(&engine,
                     &QQmlApplicationEngine::objectCreationFailed,
                     &app,
                     []() {
                         qCritical() << "Failed to load QML componrnt!";
                         QCoreApplication::exit(-1);
                     },
                     Qt::QueuedConnection);

    //  логирование предупреждений qml (qt 6.1 - 6.3)
    QObject::connect(&engine,
                     &QQmlApplicationEngine::warnings,
                     [](const QList<QQmlError> &warnings){
                         for (const auto &warn : warnings)
                         {
                             qCritical() << warn.toString();
                         }
                     });

    engine.load(url);  //   загружаем интерфейс


    return app.exec();
}
