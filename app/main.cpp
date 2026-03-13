#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <imageprocessor.h>
//#include <QQuickStyle>  //  for STATIC



int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

//    QQuickStyle::setStyle("Windows");   //  for STATIC

    QQmlApplicationEngine engine;

    //  Устанавливаем официальное имя приложения для системы
    app.setApplicationName(APP_NAME_STR);

    engine.addImageProvider(QLatin1String("opencv"), new ImageProcessor);

    const QUrl url("qrc:/qt/qml/VisualizationModule/Visualization.qml");

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
