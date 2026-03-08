#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
//#include <QtQml/qqml.h>



int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    //Q_INIT_RESOURCE(VisualizationModule);

    QQmlApplicationEngine engine;


    //engine.addImportPath("qrc:/qt/qml/");

    const QUrl url("qrc:/qt/qml/VisualizationModulePath/Visualization.qml");
    //const QUrl url("qrc:/path/Main.qml");

    engine.load(url);  //   загружаем интерфейс

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                         if (!obj && url == objUrl)
                             QCoreApplication::exit(-1);
                     }, Qt::QueuedConnection);

    if (engine.rootObjects().isEmpty())
        return -1;




    return app.exec();
}
