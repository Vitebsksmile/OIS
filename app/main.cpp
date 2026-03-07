#include <QGuiApplication>
#include <QQmlApplicationEngine>
//#include <QtQml>
//#include <QtQml/qqml.h>


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    //Q_INIT_RESOURCE(VisualizationModule);

    QQmlApplicationEngine engine;


    //engine.addImportPath("qrc:/qt/qml/");

    const QUrl url("qrc:/qt/qml/modules/VisualizationModule/MyWindow.qml");

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                         if (!obj && url == objUrl)
                             QCoreApplication::exit(-1);
                     }, Qt::QueuedConnection);

    if (engine.rootObjects().isEmpty())
        return -1;

    engine.load(url);  //   загружаем интерфейс


    return app.exec();
}
